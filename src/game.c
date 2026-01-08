#include <stddef.h>
#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "game.h"
#include "debug.h"
#include "error.h"
#include "ground.h"
#include "entity.h"
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

bool g_is_game_init = false;

#define MAX_NUM_TEXTURES 32

static inline size_t create_texture(game_t *game, const char *path_to_bmp);

struct game {
	// Game
	game_init_data_t init_data;
	SDL_Texture *textures[MAX_NUM_TEXTURES];
	const char *loaded_texture_paths[MAX_NUM_TEXTURES];
	size_t num_textures;
	SDL_Window *win;
	SDL_Renderer *ren;
	
	// Game objects
	ground_t *ground;
	entity_t *entity;
};


game_t *game_init(
	game_init_data_t game_init_data,
	ground_init_data_t ground_init_data,
	entity_init_data_t entity_init_data
) {
	// GAME

	DBG("Initializing game.");

	const char *title = game_init_data.title;
	const int win_w = game_init_data.win_w;
	const int win_h = game_init_data.win_h;
	const bool vsync = game_init_data.vsync;

	if (g_is_game_init) {
		SET_ERR("Game cannot be initialized multiple times.");
		return NULL;
	}

	game_t *game = calloc(1, sizeof(game_t));
	if (!game) {
		SET_ERR("Failed to allocate game.");
		SDL_Quit();
		DBG("SDL terminated.");
		return NULL;
	}
	DBG("Game created.");

	game->win = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		win_w,
		win_h,
		SDL_WINDOW_SHOWN
	);
	if (!game->win) {
		SET_ERR("Failed to create window.");
		game_del(game);
		return NULL;
	}
	DBG("Window created.");

	game->ren = SDL_CreateRenderer(
		game->win,
		-1,
		vsync ? SDL_RENDERER_PRESENTVSYNC : 0
	);
	if (!game->ren) {
		SET_ERR("Failed to create renderer.");
		game_del(game);
		return NULL;
	}
	DBG("Renderer created.");

	game->init_data = game_init_data;
	g_is_game_init = true;
	DBG("Game initialization finished.");

	// GROUND

	DBG("Initializing ground...");

	const size_t ground_tex_id = create_texture(
		game, ground_init_data.path_to_spritesheet_bmp
	);
	if (ground_tex_id == -1lu) {
		game_del(game);
		return NULL;
	}
	game->ground = ground_init(ground_init_data, ground_tex_id);
	if (!game->ground) {
		game_del(game);
		return NULL;
	}
	DBG("Ground initialization finished.");

	// ENTITY
	
	DBG("Initializing entity...");

	const size_t entity_tex_id = create_texture(
		game, entity_init_data.path_to_spritesheet_bmp
	);
	if (entity_tex_id == -1lu) {
		game_del(game);
		return NULL;
	}
	game->entity = entity_new(entity_init_data, entity_tex_id);
	if (!game->entity) {
		game_del(game);
		return NULL;
	}

	DBG("Entity initialization finished.");

	return game;
}

static inline size_t create_texture(game_t *game, const char *path_to_bmp) {
	if (!game || !g_is_game_init) {
		SET_ERR("Invalid argument.");
		return (size_t)-1;
	}

	for (size_t i = 0; i < game->num_textures; i++) {
		if (!strcmp(game->loaded_texture_paths[i], path_to_bmp)) {
			DBG("Texture has already been loaded.");
			return i;
		}
	}

	if (game->num_textures + 1 > MAX_NUM_TEXTURES) {
		SET_ERR("MAX_NUM_TEXTURES buffer overflow.");
		return (size_t)-1;
	}

	SDL_Surface *sur = SDL_LoadBMP(path_to_bmp);
	if (!sur) {
		SET_ERR("Failed to create surface.");
		return (size_t)-1;
	}
	DBG("Surface created.");

	SDL_Texture *tex = SDL_CreateTextureFromSurface(game->ren, sur);
	if (!tex) {
		SET_ERR("Failed to create texture.");
		SDL_FreeSurface(sur);
		DBG("Surface freed.");
		return (size_t)-1;
	}
	DBG("Texture created.");
	
	SDL_FreeSurface(sur);
	DBG("Surface freed.");

	game->textures[game->num_textures] = tex;
	game->loaded_texture_paths[game->num_textures] = path_to_bmp;
	game->num_textures++;

	return game->num_textures - 1;
}

static inline int render_objects(
	const game_t *game,
	const ground_render_data_t *ground_render_data,
	const entity_render_data_t *entity_render_data
) {
	if (	!game || !g_is_game_init ||
		!ground_render_data ||
		!entity_render_data
	) {
		SET_ERR("Invalid arguments.");
		return 1;
	}

	if (ground_render_data->tex_id >= MAX_NUM_TEXTURES) {
		SET_ERR("ground_tex_id is out of bounds.");
		return 1;
	}

	SDL_Texture *ground_tex = game->textures[ground_render_data->tex_id];
	const size_t num_blocks = ground_render_data->num_blocks;
	const block_t *blocks = ground_render_data->blocks;
	
	for (size_t i = 0; i < num_blocks; i++) {
		const block_t *block = &blocks[i];
		if (SDL_RenderCopyF(
			game->ren,
			ground_tex,
			&block->srcrect,
			&block->dstrect)
		) {
			SET_ERR("Failed to render block.");
			return 1;
		}
	}

	if (entity_render_data->tex_id >= MAX_NUM_TEXTURES) {
		SET_ERR("entity_tex_id is out of bounds.");
		return 1;
	}

	SDL_Texture *entity_tex = game->textures[entity_render_data->tex_id];

	SDL_RenderCopyF(
		game->ren,
		entity_tex,
		&entity_render_data->srcrect,
		&entity_render_data->dstrect
	);

	return 0;
}

int game_run(game_t *game) {
	if (!game || !g_is_game_init ) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	SDL_Event event;
	bool is_running = true;

	const Uint8 bg_r = game->init_data.bg_color_r;
	const Uint8 bg_g = game->init_data.bg_color_g;
	const Uint8 bg_b = game->init_data.bg_color_b;

	DBG("Entering game loop...");

	while (is_running) {
		while (SDL_PollEvent(&event)) {
			if (
				event.type == SDL_KEYDOWN &&
				event.key.keysym.sym == SDLK_q
			) {
				is_running = false;
			}
		}

		if (	SDL_SetRenderDrawColor(
				game->ren,
				bg_r, 
				bg_g,
				bg_b,
				255)
		) {
			SET_ERR("Failed to set render draw color.");
			return 1;
		}

		if (SDL_RenderClear(game->ren)) {
			SET_ERR("Failed to clear renderer.");
			return 1;
		}

		if (	render_objects(
				game,
				ground_get_render_data(game->ground),
				entity_get_render_data(game->entity))
		) {
			return 1;
		}

		SDL_RenderPresent(game->ren);
	}

	DBG("Game loop finished.");

	return 0;
}

void game_del(game_t *game) {
	if (!game) return;

	DBG("Cleaning up...");

	// Entity
	
	DBG("Cleaning up entity...");

	if (game->entity) {
		entity_del(game->entity);
	}

	DBG("Entity cleanup finished.");

	// GROUND

	DBG("Cleaning up ground...");

	if (game->ground) {
		ground_del(game->ground);
	}

	DBG("Ground cleanup finished.");
	
	// GAME

	DBG("Cleaning up game...");

	if (game->num_textures) {
		for (size_t i = 0; i < game->num_textures; i++) {
			if (game->textures[i]) {
				SDL_DestroyTexture(game->textures[i]);
			}
		}
		DBG("Textures destroyed.");
	}

	if (game->ren) {
		SDL_DestroyRenderer(game->ren);
		DBG("Renderer destroyed.");
	}

	if (game->win) {
		SDL_DestroyWindow(game->win);
		DBG("Window destroyed.");
	}

	SDL_Quit();
	DBG("SDL terminated.");

	free(game);
	
	DBG("Game deleted.");
	g_is_game_init = false;

	DBG("Game cleanup finished.");

	DBG("All cleanup finished.");
}
