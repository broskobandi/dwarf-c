#ifndef GROUND_H
#define GROUND_H

#include "init.h"
#include <SDL2/SDL_rect.h>

#define MAX_NUM_BLOCKS 1024LU

typedef struct block {
	SDL_FRect dstrect;
	SDL_FRect hitbox;
	SDL_Rect srcrect;
	size_t row;
	size_t col;
	size_t layer;
} block_t;

typedef struct ground_render_data {
	block_t blocks[MAX_NUM_BLOCKS];
	size_t num_blocks;
	size_t tex_id;
} ground_render_data_t;

typedef struct ground ground_t;

ground_t *ground_init(ground_init_data_t init_data, size_t tex_id);
int ground_update(ground_t *ground);
const block_t *ground_get_blocks(const ground_t *ground);
const ground_render_data_t *ground_get_render_data(const ground_t *ground);
size_t ground_get_num_blocks(const ground_t *ground);
size_t ground_get_tex_id(const ground_t *ground);
void ground_del(ground_t *ground);

#endif
