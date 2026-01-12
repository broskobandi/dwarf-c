#define SDL_MAIN_HANDLED
#include "game.h"
#include "ground.h"
#include "entity.h"
#include "error.h"
#include "debug.h"
#include "stddef.h"
#include <SDL2/SDL.h>
#include <string.h>

/** THe size of the textures buffer. */
#define TEXTURES_BUFF_SIZE 64lu

/** Main game object.
 * It holds all other game objects and engine components.
 * Onle one instance of this can be alove at any one given time in
 * a single thread. */
struct game {
	SDL_Texture *textures[TEXTURES_BUFF_SIZE];
	const char *loaded_texture_paths[TEXTURES_BUFF_SIZE];
	size_t num_tex;
	SDL_Window *win;
	SDL_Renderer *ren;
	ground_t *ground;
	entity_t *entity;
};

/** Static instance of the main game object. */
_Thread_local static struct game g_game;
/** Boolean-like int representing the init state of the 
 * game object instance. */
_Thread_local static int g_is_init;

/** Loads a texture from bmp and stores it in an internal buffer 
 * if it hasn't been loaded yet. Returns the index that acts 
 * as a unique texture ID (even if the texture has been loaded 
 * previously).
 * \param path_to_bmp The path to the bmp to be loaded.
 * \returns The index of the texture or (size_t)-1 on failure.
 * Sets the internal error state on failure. */
static inline size_t load_texture(const char *path_to_bmp) {
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

/** Renders the ground blocks.
 * \returns 0 on success or 1 on failure.
 * Sets the internal error state on failure. */
static inline int render_ground() {
	if (!g_is_init) {
		SET_ERR("Game must be initialized before running render_ground()");
		return 1;
	}

	const blocks_vec_t *blocks_vec = ground_get_blocks_vec(g_game.ground);
	const block_t ***blocks = (const block_t ***)blocks_vec->blocks;
	const size_t num_layers = blocks_vec->num_layers;
	const size_t num_cols = blocks_vec->num_cols;
	const size_t num_rows = blocks_vec->num_rows;
	SDL_Texture *(*textures)[TEXTURES_BUFF_SIZE] = &g_game.textures;

	for (size_t layer = 0; layer < num_layers; layer++) {
		for (size_t row = 0; row < num_rows; row++) {
			for (size_t col = 0; col < num_cols; col++) {
				const block_t *block = &blocks[layer][row][col];
				SDL_Texture *tex = (*textures)[block->tex_id];
				SDL_RenderCopyF(
					g_game.ren,
					tex,
					&block->srcrect,
					&block->dstrect
	  			);
	 		}
		}
	}

	for (size_t layer = 0; layer < num_layers; layer++) {
		for (size_t row = 0; row < num_rows; row++) {
			for (size_t col = 0; col < num_cols; col++) {
				const block_t *block = &blocks[layer][row][col];
				SDL_SetRenderDrawColor(g_game.ren, 150, 0, 0, 128);
				SDL_RenderFillRectF(g_game.ren, &block->hitbox);
			}
		}
	}

	return 0;
}

/** Initializes the game instance.
 * \param init_data The game init data.
 * \returns 0 on success or 1 on failure.
 * Sets the internal error state on failure. */
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

/** Runs the game instance. 
 * \returns 0 on success or 1 on failure.
 * Sets the internal error state on failure. */
int game_run() {
	if (!g_is_init) {
		SET_ERR("Game must be initialized before running game_run()");
		return 1;
	}

	SDL_Event event;
	int is_running = 1;

	if (SDL_SetRenderDrawBlendMode(g_game.ren, SDL_BLENDMODE_BLEND)) {
		SET_ERR("Failed to set render draw blend mode.");
		return 1;
	}

	while (is_running) {
		while (SDL_PollEvent(&event)) {
			if (	event.type == SDL_QUIT ||
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

		render_ground();

		SDL_RenderPresent(g_game.ren);
	}

	return 0;
}

/** Cleans up all memory allocated for the game instance. */
void game_quit() {
	if (g_is_init) {
		if (g_game.entity) {
			entity_del(g_game.entity);
		}
		if (g_game.ground) {
			ground_del(g_game.ground);
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
