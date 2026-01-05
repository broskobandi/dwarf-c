#include "game.h"
#include "sdl.h"
#include "error.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>

#define WIN_W 800
#define WIN_H 600

#define ASSERT(expr)\
	do {\
		if (!(expr)) {\
			game_del(game);\
			return 1;\
		}\
	} while(0)

typedef struct game {
	sdl_t sdl;
} game_t;

static inline void game_del(game_t *game) {
	if (game) {
		sdl_del(&game->sdl);
	}
}

static inline int game_init(game_t *game) {
	if (!game) {
		SET_ERR("game cannot be NULL.");
		return 1;
	}

	ASSERT(!sdl_init(&game->sdl, "Dwarf", WIN_W, WIN_H));

	return 0;
}

#undef ASSERT
#define ASSERT(expr)\
	do {\
		if (!(expr)) {\
			game_del(&game);\
			return 1;\
		}\
	} while(0)

int game_run() {
	game_t game = {0};
	ASSERT(!game_init(&game));
	SDL_Event *event = &game.sdl.event;
	SDL_Renderer *ren = game.sdl.ren;

	int is_running = 1;

	while (is_running) {
		while (SDL_PollEvent(event)) {
		}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q]) {
			is_running = 0;
		}
		ASSERT(!SDL_SetRenderDrawColor(ren, 30, 70, 70, 255));
		ASSERT(!SDL_RenderClear(ren));

		SDL_RenderPresent(ren);
	}

	game_del(&game);
	return 0;
}
