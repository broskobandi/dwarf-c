#include "game.h"
#include "error.h"

int main(void) {
	game_init_data_t init_data = {
		.title = "Dwarf",
		.win_w = 800,
		.win_h = 600,
		.vsync = 1,
		.ground_init_data = {
			.num_cols = 4,
			.num_rows = 6,
			.num_layers = 2,
			.path_to_bmp = "../assets/ground4.bmp",
			.block_size = 128.0f,
			.hitbox_size = 64.0f,
			.x_offset = 64.0f,
			.y_offset = 32.0f,
			.z_offset = 64.0f
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
