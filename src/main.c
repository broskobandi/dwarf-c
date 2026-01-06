#include <stdbool.h>
#define SDL_MAIN_HANDLED
#include "error.h"
#include "game.h"
#include <stdio.h>

#define ASSERT(expr)\
	do {\
		if (!(expr)) {\
			game_del(&game);\
			fprintf(stderr, "%s\n", get_err());\
			return 1;\
		}\
	} while (0)

int main(void) {

	game_t game = {0};
	if (game_init(&game)) {
		fprintf(stderr, "%s\n", get_err());
		return 1;
	}

	SDL_Event event;
	bool is_running = true;

	while (is_running) {
		while (SDL_PollEvent(&event)) {
		}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q]) is_running = false;
		ASSERT(!SDL_SetRenderDrawColor(game.ren, 30, 70, 70, 255));
		ASSERT(!SDL_RenderClear(game.ren));

		SDL_RenderPresent(game.ren);
	}

	game_del(&game);
	return 0;
}
