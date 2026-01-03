#include "game.h"
#include "error.h"
#include "sdl.h"
#include <SDL2/SDL_keyboard.h>

#define ASSERT(expr)\
	do {\
		if (!(expr)) {\
			game_del(game);\
			return 1;\
		}\
	} while(0)

typedef struct game {
	sdl_t *sdl;
} game_t;

static inline void game_del(game_t *game) {
	if (game) {
		sdl_del(game->sdl);
	}
}

static inline int game_init(game_t *game) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	game->sdl = sdl_new("Dwarf", 800, 600);
	ASSERT(game->sdl);

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
	if (game_init(&game)) return 1;

	sdl_t *sdl = game.sdl;

	int is_running = 1;

	while (is_running) {
		int event = -1;
		while ((event = sdl_poll_event(sdl))) {
			ASSERT(event != -1);
		}

		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q]) is_running = 0;

		SDL_Color bg = {30, 70, 70, 255};
		ASSERT(!sdl_set_draw_color(sdl, bg));
		ASSERT(!sdl_render_clear(sdl));

		ASSERT(!sdl_render_present(sdl));
	}

	sdl_del(sdl);
	return 0;
}
