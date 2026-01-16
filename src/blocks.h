#ifndef BLOCKS_H
#define BLOCKS_H

#include "game.h"
#include <SDL2/SDL_rect.h>

typedef struct block {
	SDL_FRect dstrect;
	SDL_FRect hitbox;
	SDL_Rect srcrect;
	int has_left_shadow;
	int has_right_shadow;
	int has_right_light;
	int has_top_shadow;
	int is_highlighted;
	int is_selected;
	int is_active;
	int is_visible;
} block_t;

typedef struct blocks {
	block_t ***blocks;
	blocks_init_data_t init_data;
	size_t tex_id;
} blocks_t;

int blocks_init(
	blocks_t *blocks,
	blocks_init_data_t init_data,
	size_t tex_id
);

void blocks_del(blocks_t *blocks);

#endif
