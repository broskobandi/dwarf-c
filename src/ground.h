#ifndef GROUND_H
#define GROUND_H

#include "init.h"
#include <SDL2/SDL_rect.h>


typedef struct block {
	SDL_FRect dstrect;
	SDL_FRect hitbox;
	SDL_Rect srcrect;
	size_t row;
	size_t col;
	size_t layer;
} block_t;

typedef struct ground ground_t;

ground_t *ground_init(ground_init_data_t init_data, size_t tex_id);
int ground_update(ground_t *ground);
const block_t *ground_get_blocks(const ground_t *ground);
size_t ground_get_num_blocks(const ground_t *ground);
size_t ground_get_tex_id(const ground_t *ground);
void ground_del(ground_t *ground);

#endif
