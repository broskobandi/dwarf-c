#ifndef GAME_H
#define GAME_H

#include "debug.h"
#include "error.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#define WIN_W 800
#define WIN_H 600

typedef struct game {
	SDL_Window *win;
	SDL_Renderer *ren;
} game_t;

int game_init(game_t *game);
void game_del(game_t *game);

#endif
