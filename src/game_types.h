#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>

typedef struct game {
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Event event;
	int is_sdl_init;
	int is_running;
	SDL_Color bg_col;
} game_t;

#endif
