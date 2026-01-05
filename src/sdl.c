#include "sdl.h"
#include "error.h"
#include "debug.h"
#include <SDL2/SDL.h>

int sdl_init(sdl_t *sdl, const char *title, int w, int h) {
	if (!sdl) {
		SET_ERR("sdl cannot be NULL.");
		return 1;
	}
	
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SET_ERR("Failed to init SDL.");
		return 1;
	}
	DBG("SDL initialized.");

	sdl->win = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h, SDL_WINDOW_SHOWN
	);
	if (!sdl->win) {
		SET_ERR("Failed to create window.");
		return 1;
	}
	DBG("Window created.");

	sdl->ren = SDL_CreateRenderer(sdl->win, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!sdl->ren) {
		SET_ERR("Failed to create renderer.");
		return 1;
	}
	DBG("Renderer created.");

	return 0;
}

void sdl_del(sdl_t *sdl) {
	if (sdl) {
		if (sdl->ren) {
			SDL_DestroyRenderer(sdl->ren);
			DBG("Renderer desrtoyed.");
		}
		if (sdl->win) {
			SDL_DestroyWindow(sdl->win);
			DBG("Window desrtoyed.");
		}
		SDL_Quit();
		DBG("SDL terminated.");
	}
}
