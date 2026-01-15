#include "game.h"
#include <stdio.h>

#define WIN_W 800
#define WIN_H 600

int main(void) {
	game_init_data_t game = {
		.title = "Dwarf",
		.win_w = WIN_W,
		.win_h = WIN_H,
		.has_vsync = 1,
		.bg_r = 30,
		.bg_g = 75,
		.bg_b = 75
	};

	if (game_init(game)) {
		fprintf(stderr, "%s\n", game_get_error());
		return 1;
	}

	if (game_run()) {
		game_quit();
		fprintf(stderr, "%s\n", game_get_error());
		return 1;
	}

	game_quit();

	return 0;
}
