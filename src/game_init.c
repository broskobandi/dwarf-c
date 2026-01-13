#include "game_init.h"
#include "error.h"
#include "game_types.h"
#include "debug.h"
#include <SDL2/SDL.h>

int game_init_sdl(game_t *game) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SET_ERR("Failed to init SDL.");
		return 1;
	}

	game->is_sdl_init = 1;
	
	DBG("SDL initialized.");

	return 0;
}

int game_init_win(game_t *game, const char *title, int w, int h) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	if (!game->is_sdl_init) {
		SET_ERR("SDL must be initialized first.");
		return 1;
	}

	game->win = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		SDL_WINDOW_SHOWN
	);
	if (!game->win) {
		SET_ERR("Failed to create window.");
		return 1;
	}
	DBG("Window created.");

	return 0;
}

int game_init_ren(game_t *game, int has_vsync) {
	if (!game) {
		SET_ERR("SDL argument.");
		return 1;
	}

	if (!game->is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	game->ren = SDL_CreateRenderer(game->win, -1,
		has_vsync ? SDL_RENDERER_PRESENTVSYNC : 0
	);
	if (!game->ren) {
		SET_ERR("Failed to create renderer.");
		return 1;
	}
	DBG("Renderer created.");

	if (SDL_SetRenderDrawBlendMode(game->ren, SDL_BLENDMODE_BLEND)) {
		SET_ERR("Failed to set render draw blendmode.");
		return 1;
	}
	DBG("Render draw blend mode set.");

	return 0;
}

int game_init_variables(game_t *game, uint8_t r, uint8_t g, uint8_t b) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	if (!game->is_sdl_init) {
		SET_ERR("SDL must be initialized first.");
		return 1;
	}

	game->is_running = 1;
	game->bg_col.r = r;
	game->bg_col.g = g;
	game->bg_col.b = b;
	game->bg_col.a = 255;

	return 0;
}
