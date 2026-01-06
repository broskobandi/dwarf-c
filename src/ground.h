#ifndef GROUND_H
#define GROUND_H

#include "debug.h"
#include "types.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#define MAX_NUM_TILES 1024LU

typedef struct tile {
	SDL_FRect dstrect;
	SDL_Rect srcrect;
	SDL_FRect hitbox;
	coordinates_t top;
	bool is_blocked_from_above;
	bool is_highlighted;
	bool is_selected;
} tile_t;
	
typedef struct ground {
	const int num_cols;
	const int num_rows;
	const int num_layers;
	const float dstrect_size;
	const int srcrect_size;
	const float hitbox_size;
	const float spatial_offset;
	const float z_offset;
	SDL_Texture * const tex;
	size_t num_tiles;
	int num_tiles_per_layer;
	tile_t tiles[MAX_NUM_TILES];
} ground_t;

int ground_init(ground_t *ground);
int ground_update(ground_t *ground, SDL_Point mouse, bool left_click);
void ground_del(ground_t *ground);

#endif
