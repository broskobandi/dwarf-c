#include "game.h"
#include "error.h"

#define WIN_W 800
#define WIN_H 600
#define BLOCK_SIZE 128.0f

int main(void) {
	game_init_data_t init_data = {
		.title = "Dwarf",
		.win_w = WIN_W,
		.win_h = WIN_H,
		.vsync = 1,
		.ground_init_data = {
			.num_cols = 4,
			.num_rows = 6,
			.num_layers = 2,
			.path_to_bmp = "../assets/ground4.bmp",
			.block_size = BLOCK_SIZE,
			.img_size = 32,
			.hitbox_size = BLOCK_SIZE / 2 - 1,
			.x_offset = BLOCK_SIZE / 2,
			.y_offset = BLOCK_SIZE / 4,
			.z_offset = BLOCK_SIZE / 2,
			.origin_x = (float)WIN_W / 2 - BLOCK_SIZE / 2,
			.origin_y = 0.0f,
			.origin_z = (float)WIN_H / 4,
			.right_shadow_img_index = 1,
			.left_shadow_img_index = 4,
			.right_light_img_index = 2,
			.front_corner_img_index = 3,
			.top_shadow_img_index = 5,
			.highlight_img_index = 6,
			.select_img_index = 7
		},
		.entity_init_data = {
			.size = 64.0f,
			.path_to_bmp = "../assets/dwarf10.bmp",
			.hitbox_size = 64.0f
		}
	};
	if (game_init(init_data)) {
		print_err();
		game_quit();
		return 1;
	}
	if (game_run()) {
		print_err();
		game_quit();
		return 1;
	}

	game_quit();
	return 0;
}
