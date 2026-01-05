#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

typedef struct sdl {
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Event event;
} sdl_t;

int sdl_init(sdl_t *sdl, const char *title, int w, int h);
void sdl_del(sdl_t *sdl);

#endif
