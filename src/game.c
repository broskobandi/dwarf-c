#define SDL_MAIN_HANDLED
#include "game.h"
#include "debug.h"
#include "error.h"
#include "game_types.h"
#include "game_init.h"
#include "game_quit.h"
#include "game_run.h"

game_t g_game;

void game_print_error() {
	print_err();
}

int game_init(game_init_data_t init_data) {

	if (game_init_sdl(&g_game))
		return 1;

	if (game_init_win(&g_game, init_data.title, init_data.win_w, init_data.win_h))
		return 1;

	if (game_init_ren(&g_game, init_data.has_vsync))
		return 1;

	if (game_init_variables(&g_game,
		init_data.background_r,
		init_data.background_g,
		init_data.background_b)
	) {
		return 1;
	}

	return 0;
}

int game_run() {
	while (g_game.is_running) {
		if (game_poll_events(&g_game)) return 1;
		if (game_clear(&g_game)) return 1;
		if (game_present(&g_game)) return 1;
	}

	return 0;
}

void game_quit() {
	game_destroy_ren(&g_game);
	game_destroy_win(&g_game);
	game_terminate_sdl(&g_game);
}
