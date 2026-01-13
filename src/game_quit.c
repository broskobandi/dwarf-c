#include "game_quit.h"
#include "debug.h"
#include <SDL2/SDL.h>

void game_destroy_ren(game_t *game) {
	if (game->ren) {
		SDL_DestroyRenderer(game->ren);
		DBG("Renderer destroyed.");
	}
}

void game_destroy_win(game_t *game) {
	if (game->win) {
		SDL_DestroyWindow(game->win);
		DBG("Window destroyed.");
	}
}

void game_terminate_sdl(game_t *game) {
	if (game->is_sdl_init) {
		SDL_Quit();
		DBG("SDL terminated.");
	}
}
