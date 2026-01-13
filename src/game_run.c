#include "game_run.h"
#include "game_types.h"
#include "error.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

int game_poll_events(game_t *game) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	if (!game->is_sdl_init || !game->win || !game->ren) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	while (SDL_PollEvent(&game->event)) {
		if (	game->event.type == SDL_QUIT ||
			(game->event.type == SDL_KEYDOWN &&
			 game->event.key.keysym.sym == SDLK_q)
		) {
			game->is_running = 0;
		}
	}

	return 0;
}

int game_set_draw_color(game_t *game, SDL_Color col) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	if (!game->is_sdl_init || !game->win || !game->ren) {
		SET_ERR("Window and renderer must be initialized first.");
		return 1;
	}

	if (SDL_SetRenderDrawColor(game->ren, col.r, col.g, col.b, col.a)) {
		SET_ERR("Failed to set render draw color.");
		return 1;
	}

	return 0;
}

int game_clear(game_t *game) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	if (!game->is_sdl_init || !game->win || !game->ren) {
		SET_ERR("Window and renderer must be initialized first.");
		return 1;
	}

	if (!game->is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	if (game_set_draw_color(game, game->bg_col)) return 1;

	if (SDL_RenderClear(game->ren)) {
		SET_ERR("Failed to clear renderer.");
		return 1;
	}

	return 0;
}

int game_present(game_t *game) {
	if (!game) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	if (!game->is_sdl_init || !game->win || !game->ren) {
		SET_ERR("Window and renderer must be initialized first.");
		return 1;
	}

	if (!game->is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	SDL_RenderPresent(game->ren);

	return 0;
}
