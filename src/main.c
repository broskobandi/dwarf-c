#include "game.h"
#include <stdio.h>

#define WIN_W 800
#define WIN_H 600
#define BLOCK_SIZE 128.0f

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

	blocks_init_data_t blocks = {
		.path_to_bmp = "assets/ground4.bmp",
		.block_size = BLOCK_SIZE,
		.hitbox_size = BLOCK_SIZE / 2 - 1,
		.img_size = 32,
		.origin_x = (float)(WIN_W) / 2 - BLOCK_SIZE / 2,
		.origin_y = 0,
		.origin_z = 0,
		.x_offset = BLOCK_SIZE / 2,
		.y_offset = BLOCK_SIZE / 4,
		.z_offset = BLOCK_SIZE / 2,
		.num_layers = 2,
		.num_rows = 6,
		.num_cols = 4,
		.left_shadow_index = 4,
		.right_shadow_index = 1,
		.right_light_index = 2,
		.top_shadow_index = 5,
		.selected_index = 7,
		.highlighted_index = 6
	};

	if (game_init(game, blocks)) {
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
