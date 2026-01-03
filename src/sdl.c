#include "sdl.h"
#include "debug.h"
#include "error.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

#define IF_SDL_INVALID_RETURN(return_value)\
	if (!sdl || !sdl->win || !sdl->ren) {\
		SET_ERR("Invalid sdl object passed as argument.");\
		return (return_value);\
	}

struct sdl {
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Event event;
};

sdl_t *sdl_new(const char *title, int w, int h) {
	sdl_t *sdl = calloc(1, sizeof(sdl_t));
	if (!sdl) {
		SET_ERR("Failed to allocate sdl_t.");
		return NULL;
	}
	DBG("sdl_t allocated.");

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SET_ERR("Failed to init sdl.");
		sdl_del(sdl);
		DBG("SDL terminated.");
		return NULL;
	}
	DBG("SDL initialized.");

	sdl->win = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w, h,
		SDL_WINDOW_SHOWN
	);
	if (!sdl->win) {
		SET_ERR("Failed to create SDL_Window.");
		sdl_del(sdl);
		return NULL;
	}
	DBG("SDL_Window created.");

	sdl->ren = SDL_CreateRenderer(sdl->win, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!sdl->ren) {
		SET_ERR("Failed to create SDL_Renderer.");
		sdl_del(sdl);
		return NULL;
	}
	DBG("SDL_Renderer created.");

	return sdl;
}

void sdl_del(sdl_t *sdl) {
	if (sdl) {
		if (sdl->ren) {
			SDL_DestroyRenderer(sdl->ren);
			DBG("SDL_Renderer destroyed.");
		}
		if (sdl->win) {
			SDL_DestroyWindow(sdl->win);
			DBG("SDL_Window destroyed.");
		}
		SDL_Quit();
		DBG("SDL terminated.");
		free(sdl);
		DBG("sdl_t freed.");
	}
}

int sdl_poll_event(sdl_t *sdl) {
	IF_SDL_INVALID_RETURN(-1);

	return SDL_PollEvent(&sdl->event);
}

int sdl_has_event_type(const sdl_t *sdl, SDL_EventType type) {
	IF_SDL_INVALID_RETURN(-1);

	return sdl->event.type == type;
}

int sdl_has_keycode(const sdl_t *sdl, SDL_Keycode keycode) {
	IF_SDL_INVALID_RETURN(-1)

	return sdl->event.key.keysym.sym == keycode;
}

int sdl_set_draw_color(const sdl_t *sdl, SDL_Color color) {
	IF_SDL_INVALID_RETURN(1);

	if (SDL_SetRenderDrawColor(sdl->ren, color.r, color.g, color.b, color.a)) {
		SET_ERR("Failed to set render draw color.");
		return 1;
	}

	return 0;
}

int sdl_render_clear(const sdl_t *sdl) {
	IF_SDL_INVALID_RETURN(1);

	if (SDL_RenderClear(sdl->ren)) {
		SET_ERR("Failed to clear renderer.");
		return 1;
	}

	return 0;
}

int sdl_render_present(const sdl_t *sdl) {
	IF_SDL_INVALID_RETURN(1);

	SDL_RenderPresent(sdl->ren);

	return 0;
}
