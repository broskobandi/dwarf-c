#define SDL_MAIN_HANDLED
#include "game.h"
#include "error.h"
#include "debug.h"
#include <SDL2/SDL.h>

typedef struct game {
	game_init_data_t init_data;
	SDL_Window *win;
	SDL_Renderer *ren;
	int is_sdl_init;
} game_t;

_Thread_local static game_t g_game;

static inline int set_draw_color(SDL_Color col);

int game_init(game_init_data_t game_init_data) {
	if (g_game.is_sdl_init) {
		SET_ERR("Cannot initialize game twice.");
		return 1;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SET_ERR("Failed to initialize SDL.");
		return 1;
	}
	g_game.is_sdl_init = 1;
	DBG("SDL initialized.");

	g_game.win = SDL_CreateWindow(
		game_init_data.title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		game_init_data.win_w,
		game_init_data.win_h,
		SDL_WINDOW_SHOWN
	);
	if (!g_game.win) {
		SET_ERR("Failed to create window.");
		game_quit();
		return 1;
	}
	DBG("Window created.");

	g_game.ren = SDL_CreateRenderer(
		g_game.win,
		-1,
		game_init_data.has_vsync ? SDL_RENDERER_PRESENTVSYNC : 0
	);
	if (!g_game.ren) {
		SET_ERR("Failed to create renderer.");
		game_quit();
		return 1;
	}
	DBG("Renderer created.");

	if (SDL_SetRenderDrawBlendMode(g_game.ren, SDL_BLENDMODE_BLEND)) {
		SET_ERR("Failed to set render draw blend mode.");
		game_quit();
		return 1;
	}

	g_game.init_data = game_init_data;
	return 0;
}

int game_run() {
	if (!g_game.is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	SDL_Event event;
	int is_running = 1;
	const SDL_Color bg = {
		.r = g_game.init_data.bg_r,
		.g = g_game.init_data.bg_g,
		.b = g_game.init_data.bg_b,
		255
	};

	DBG("Running main loop...");

	while (is_running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT ||
			   (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_q)
			) {
				is_running = 0;
			}
		}

		if (set_draw_color(bg)) {
			return 1;
		}

		if (SDL_RenderClear(g_game.ren)) {
			SET_ERR("Failed to clear renderer.");
			return 1;
		}

		SDL_RenderPresent(g_game.ren);

	}

	DBG("Main loop finished.");

	return 0;
}

static inline int set_draw_color(SDL_Color col) {
	if (!g_game.is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	if (SDL_SetRenderDrawColor(g_game.ren, col.r, col.g, col.b, col.a)) {
		SET_ERR("Failed to set render draw color.");
		return 1;
	}

	return 0;
}

const char *game_get_error() {
	return get_err();
}

void game_quit() {
	if (g_game.ren) {
		SDL_DestroyRenderer(g_game.ren);
		DBG("Renderer destroyed.");
	}
	if (g_game.win) {
		SDL_DestroyWindow(g_game.win);
		DBG("Window destroyed.");
	}
	if (g_game.is_sdl_init) {
		SDL_Quit();
		DBG("SDL terminated.");
	}
}
