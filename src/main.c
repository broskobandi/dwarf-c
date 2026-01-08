#include "error.h"
#include "game.h"
#include <stdio.h>

// Game init data
#define GAME_TITLE "Dwarf"
#define GAME_WIN_W 800
#define GAME_WIN_H 600
#define GAME_VSYNC true
#define GAME_BG_COLOR_R 30
#define GAME_BG_COLOR_G 70
#define GAME_BG_COLOR_B 70

// Ground init data
#define GROUND_IN_GAME_SPRITE_SIZE 128
#define GROUND_ORIGIN_X (float)GAME_WIN_W / 2 - GROUND_IN_GAME_SPRITE_SIZE / 2
#define GROUND_ORIGIN_Y 0
// WE NEED A GROUND_ORIGIN_Z here
#define GROUND_NUM_LAYERS 2
#define GROUND_NUM_COLS 6
#define GROUND_NUM_ROWS 4
#define GROUND_HITBOX_SIZE GROUND_IN_GAME_SPRITE_SIZE / 2 // the default pos
														  // of the hitbox is
														  // the top center 
														  // of the block
#define GROUND_PATH_TO_SPRITESHEET_BMP "assets/ground4.bmp"
#define GROUND_REAL_IMG_SIZE 32
#define GROUND_BASE_IMG 0
#define GROUND_RIGHT_SHADOW_IMG 0
#define GROUND_LEFT_SHADOW_IMG 0
#define GROUND_TOP_SHADOW_IMG 0
#define GROUND_RIGHT_LIGHT_IMG 0
#define GROUND_HIGHLIGHT_IMG 0
#define GORUND_SELECT_IMG 0
#define GROUND_X_OFFSET (float)GROUND_IN_GAME_SPRITE_SIZE / 2
#define GROUND_Y_OFFSET (float)GROUND_IN_GAME_SPRITE_SIZE / 2
#define GROUND_Z_OFFSET (float)GROUND_IN_GAME_SPRITE_SIZE / 4
/* #define GROUND_Z_OFFSET GROUND_X_OFFSET / 2.0f */

int main(void) {
	game_init_data_t game_init_data = {
		.title = GAME_TITLE,
		.win_w = GAME_WIN_W,
		.win_h = GAME_WIN_H,
		.vsync = GAME_VSYNC,
		.bg_color_r = GAME_BG_COLOR_R,
		.bg_color_g = GAME_BG_COLOR_G,
		.bg_color_b = GAME_BG_COLOR_B
	};
	ground_init_data_t ground_init_data = {
		.origin_x = GROUND_ORIGIN_X,
		.origin_y = GROUND_ORIGIN_Y,
		.num_layers = GROUND_NUM_LAYERS,
		.num_cols = GROUND_NUM_COLS,
		.num_rows = GROUND_NUM_ROWS, 
		.in_game_sprite_size = GROUND_IN_GAME_SPRITE_SIZE,
		.path_to_spritesheet_bmp = GROUND_PATH_TO_SPRITESHEET_BMP,
		.real_img_size = GROUND_REAL_IMG_SIZE,
		.base_img = GROUND_BASE_IMG,
		.right_shadow_img = GROUND_RIGHT_SHADOW_IMG,
		.left_shadow_img = GROUND_LEFT_SHADOW_IMG,
		.top_shadow_img = GROUND_TOP_SHADOW_IMG,
		.right_light_img = GROUND_RIGHT_LIGHT_IMG,
		.highlight_img = GROUND_HIGHLIGHT_IMG,
		.select_img = GORUND_SELECT_IMG,
		.x_offset = GROUND_X_OFFSET,
		.y_offset = GROUND_Y_OFFSET,
		.z_offset = GROUND_Z_OFFSET
	};
	game_t *game = game_init(
		game_init_data,
		ground_init_data
	);
	if (!game) {
		fprintf(stderr, "%s\n", get_err());
		return 1;
	}
	if (game_run(game)) {
		fprintf(stderr, "%s\n", get_err());
		game_del(game);
		return 1;
	}

	game_del(game);
	return 0;
}
