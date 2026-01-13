#include "game.h"
#include "debug.h"
#include "error.h"
#include <SDL2/SDL.h>

typedef struct game {
	game_init_data_t init_data;
	SDL_Window *win;
	SDL_Window *ren;
	int is_init;
} game_t;

game_t g_game;

void game_print_error() {
	print_err();
}

int game_init(game_init_data_t init_data) {
	g_game.init_data = init_data;

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SET_ERR("Failed to init SDL.");
		return 1;
	}
	g_game.is_init = 1;
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

	return 0;
}

int game_run() {
	return 0;
}

void game_quit() {
	if (!g_game.is_init) return;
	if (g_game.win) {
		SDL_DestroyWindow(g_game.win);
		DBG("Window destroyed.");
	}
	SDL_Quit();
	DBG("SDL terminated.");
}
