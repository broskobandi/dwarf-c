#include "game.h"
#include "sdl.h"
#include "error.h"
#include "tiles.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdbool.h>

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
	tiles_t tiles;
} game_t;

static inline void game_del(game_t *game) {
	if (game) {
		tiles_del(&game->tiles);
		sdl_del(&game->sdl);
	}
}

static inline int game_init(game_t *game) {
	if (!game) {
		SET_ERR("game cannot be NULL.");
		return 1;
	}

	ASSERT(!sdl_init(&game->sdl, "Dwarf", WIN_W, WIN_H));

	const tiles_init_data_t tiles_init_data = {
		.dstrect_size = 128,
		.srcrect_size = 32,
		.hitbox_size = 64,
		.num_cols = WIN_W / 128,
		.num_rows = WIN_H / (128 / 4),
		.y_offset = 128 / 4,
		.z_offset = 128 / 2,
		.num_layers = 4
	};
	ASSERT(!tiles_init(
		&game->tiles,
		game->sdl.ren,
		"assets/ground4.bmp",
		tiles_init_data)
	);

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
	ASSERT(!SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND));

	tiles_t *tiles = &game.tiles;

	bool is_running = true;

	while (is_running) {
		bool left_click = false;
		while (SDL_PollEvent(event)) {
			if (event->type == SDL_MOUSEBUTTONDOWN) {
				if (event->button.button == SDL_BUTTON_LEFT) {
					left_click = true;
				}
			}
		}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q]) {
			is_running = false;
		}
		SDL_Point mouse_pos = {0};
		SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

		ASSERT(!tiles_update(tiles, mouse_pos, left_click));

		ASSERT(!SDL_SetRenderDrawColor(ren, 30, 70, 70, 255));
		ASSERT(!SDL_RenderClear(ren));

		ASSERT(!tiles_draw(tiles, ren));

		SDL_RenderPresent(ren);
	}

	game_del(&game);
	return 0;
}
