#ifndef INIT_H
#define INIT_H

#include <stdbool.h>
#include <stddef.h>

// GAME INIT DATA
typedef struct game_int_data { 
	const char *title;
	unsigned char bg_color_r;
	unsigned char bg_color_g;
	unsigned char bg_color_b;
	int win_w;
	int win_h;
	bool vsync;
} game_init_data_t;

// GROUND INIT DATA
typedef struct ground_init_data {
	float origin_x;
	float origin_y;
	size_t num_layers;
	size_t num_cols;
	size_t num_rows;
	float in_game_sprite_size;
	float hitbox_size;
	const char *path_to_spritesheet_bmp;
	int real_img_size;
	float x_offset;
	float y_offset;
	float z_offset;

	// zero based indices of the relevant images in the spritesheet:
	int base_img; 
	int right_shadow_img;
	int left_shadow_img;
	int top_shadow_img;
	int right_light_img;
	int highlight_img;
	int select_img;
} ground_init_data_t;

// ENTITY INIT DATA
typedef struct entity_init_data {
	float x, y, z; // starting position
	float in_game_sprite_size;
	float hitbox_size;
	const char *path_to_spritesheet_bmp;
	size_t num_imgs;
	int real_img_size;
	unsigned int ticks_per_img_update;
	float pixels_per_frame;
} entity_init_data_t;

#endif
