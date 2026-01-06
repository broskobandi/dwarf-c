#include "game.h"
#include "debug.h"
#include "error.h"

int game_init(game_t *game) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "Failed to init SDL.");
		return 1;
	}
	DBG("SDL initialized.");

	game->win = SDL_CreateWindow("Dwarf",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_W, WIN_H,
		SDL_WINDOW_SHOWN
	);
	if (!game->win) {
		SET_ERR("Failed to create window.");
		game_del(game);
		return 1;
	}
	DBG("Window created.");

	game->ren = SDL_CreateRenderer(game->win, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!game->ren) {
		SET_ERR("Failed to create renderer.");
		game_del(game);
		return 1;
	}
	DBG("Renderer created.");

	if (SDL_SetRenderDrawBlendMode(game->ren, SDL_BLENDMODE_BLEND)) {
		SET_ERR("Failed to set renderer draw blend mode.");
		game_del(game);
		return 1;
	}
	DBG("Renderer draw blendmode set.");

	return 0;
}

void game_del(game_t *game) {
	DBG("Cleaning up...");
	if (game) {
		if (game->win) {
			SDL_DestroyWindow(game->win);
			DBG("Window destroyed.");
		}
		if (game->ren) {
			SDL_DestroyRenderer(game->ren);
			DBG("Renderer destroyed.");
		}
		SDL_Quit();
		DBG("SDL terminated.");
	}
	DBG("Cleanup finished.");
}
