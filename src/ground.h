#ifndef GROUND_H
#define GROUND_H

#include <SDL2/SDL_rect.h>

#define ROWS_BUFF_SIZE 128
#define COLS_BUFF_SIZE 128
#define LAYERS_BUFF_SIZE 16

#include <stddef.h>

typedef struct block {
	SDL_FRect hitbox;
	SDL_FRect dstrect;
	SDL_Rect srcrect;
	size_t tex_id;
} block_t;

typedef struct blocks_vec {
	block_t ***blocks;
	size_t num_layers;
	size_t num_cols;
	size_t num_rows;
	int is_init;
} blocks_vec_t;

typedef struct ground_init_data {
	size_t num_cols;
	size_t num_rows;
	size_t num_layers;
	char *path_to_bmp;
	float block_size;
	int img_size;
	float hitbox_size;
	float x_offset;
	float y_offset;
	float z_offset;
	float origin_x;
	float origin_y;
	float origin_z;
} ground_init_data_t;

typedef struct ground ground_t;

ground_t *ground_new(ground_init_data_t init_data, size_t tex_id);
int ground_update(ground_t *ground);
const blocks_vec_t *ground_get_blocks_vec(const ground_t *ground);
void ground_del(ground_t *ground);

#endif
