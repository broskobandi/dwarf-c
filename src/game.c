#define SDL_MAIN_HANDLED
#include "game.h"
#include "ground.h"
#include "entity.h"
#include "error.h"
#include "debug.h"
#include "stddef.h"
#include <SDL2/SDL.h>
#include <string.h>

#define TEXTURES_BUFF_SIZE 64

struct game {
	SDL_Texture *textures[TEXTURES_BUFF_SIZE];
	const char *loaded_texture_paths[TEXTURES_BUFF_SIZE];
	size_t num_tex;
	SDL_Window *win;
	SDL_Renderer *ren;
	ground_t *ground;
	entity_t *entity;
};

_Thread_local static struct game g_game;
_Thread_local static int g_is_init;

size_t load_texture(const char *path_to_bmp) {
	if (!g_is_init) {
		SET_ERR("Game must be initialized before using load_texture().");
		return (size_t)-1;
	}

	if (!path_to_bmp) {
		SET_ERR("Invalid argument.");
		return (size_t)-1;
	}

	for (size_t i = 0; i < g_game.num_tex; i++) {
		if (!strcmp(g_game.loaded_texture_paths[i], path_to_bmp)) {
			DBG("Texture has already been loaded.");
			return i;
		}
	}

	if (g_game.num_tex + 1 > TEXTURES_BUFF_SIZE) {
		SET_ERR("TEXTURES_BUFF_SIZE overflow.");
		return (size_t)-1;
	}

	SDL_Surface *sur = SDL_LoadBMP(path_to_bmp);
	if (!sur) {
		SET_ERR("Failed to create surface.");
		return (size_t)-1;
	}
	DBG("Surface created.");

	g_game.textures[g_game.num_tex] =
		SDL_CreateTextureFromSurface(g_game.ren, sur);
	if (!g_game.textures[g_game.num_tex]) {
		SET_ERR("Failed to create texture.");
		SDL_FreeSurface(sur);
		DBG("Surface freed.");
		return (size_t)-1;
	}
	DBG("Texture created.");

	g_game.num_tex++;

	SDL_FreeSurface(sur);
	DBG("Surface freed.");

	return g_game.num_tex - 1;
}

int game_init(game_init_data_t init_data) {
	if (g_is_init) {
		SET_ERR("Game cannot be initialized twice.");
		return 1;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SET_ERR("Failed to initialize SDL.");
		return 1;
	}
	DBG("SDL initialized.");

	g_game.win = SDL_CreateWindow(
		init_data.title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		init_data.win_w,
		init_data.win_h,
		SDL_WINDOW_SHOWN
	);
	if (!g_game.win) {
		SET_ERR("Failed to create window.");
		return 1;
	}
	DBG("Window created.");

	g_game.ren = SDL_CreateRenderer(
		g_game.win,
		-1,
		init_data.vsync ? SDL_RENDERER_PRESENTVSYNC : 0
	);
	if (!g_game.ren) {
		SET_ERR("Failed to create renderer.");
		return 1;
	}
	DBG("Renderer created.");

	g_is_init = 1;

	size_t ground_tex_id = load_texture(init_data.ground_init_data.path_to_bmp);
	if (ground_tex_id == -1lu) {
		return 1;
	}
	
	g_game.ground = ground_new(init_data.ground_init_data, ground_tex_id);
	if (!g_game.ground) {
		return 1;
	}

	g_game.entity = entity_new(init_data.entity_init_data);
	if (!g_game.entity) {
		return 1;
	}

	return 0;
}

int game_run() {
	if (!g_is_init) {
		SET_ERR("Game must be initialized before running game_run()");
		return 1;
	}

	SDL_Event event;
	int is_running = 1;

	while (is_running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT ||
			   (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_q)
			) {
				is_running = 0;
			}
		}
		if (SDL_SetRenderDrawColor(g_game.ren, 30, 70, 70, 255)) {
			SET_ERR("Failed to set render draw color.");
			return 1;
		}
		if (SDL_RenderClear(g_game.ren)) {
			SET_ERR("Failed to clear renderer.");
			return 1;
		}

		SDL_RenderPresent(g_game.ren);
	}

	return 0;
}

void game_quit() {
	if (g_is_init) {
		if (g_game.ground) {
			ground_del(g_game.ground);
		}
		if (g_game.entity) {
			entity_del(g_game.entity);
		}
		if (g_game.num_tex) {
			for (size_t i = 0; i < g_game.num_tex; i++) {
				if (g_game.textures[i]) {
					SDL_DestroyTexture(g_game.textures[i]);
				}
			}
			DBG("Textures destroyed.");
		}
		if (g_game.ren) {
			SDL_DestroyRenderer(g_game.ren);
			DBG("Renderer destroyed.");
		}
		if (g_game.win) {
			SDL_DestroyWindow(g_game.win);
			DBG("Window destroyed.");
		}
		SDL_Quit();
		DBG("SDL terminated.");
		g_is_init = 0;
	}
}
