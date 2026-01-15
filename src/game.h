#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include <stdint.h>

typedef struct game_init_data {
	const char *title;
	int win_w;
	int win_h;
	int has_vsync;
	uint8_t bg_r;
	uint8_t bg_g;
	uint8_t bg_b;
} game_init_data_t;

typedef struct blocks_init_data {
	const char *path_to_bmp;
	float block_size;
	float hitbox_size;
	int img_size;
	float origin_x;
	float origin_y;
	float origin_z;
	float x_offset;
	float y_offset;
	float z_offset;
	size_t num_layers;
	size_t num_rows;
	size_t num_cols;
	size_t left_shadow_index;
	size_t right_shadow_index;
	size_t top_shadow_index;
	size_t right_light_index;
	size_t selected_index;
	size_t highlighted_index;
} blocks_init_data_t;

int game_init(
	game_init_data_t game_init_data,
	blocks_init_data_t blocks_init_data
);
int game_run();
const char *game_get_error();
void game_quit();

#endif
