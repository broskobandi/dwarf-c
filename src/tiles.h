#ifndef TILES_H
#define TILES_H

#include <stdbool.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#define MAX_NUM_TILES 1024

typedef struct tile {
	SDL_Rect dstrect;
	SDL_Rect srcrect;
	SDL_Rect hitbox;
	bool is_staggered;
	bool is_highlighted;
	bool is_covered_from_above;
	bool is_covered_from_left_down;
	bool is_covered_from_right_down;
	bool is_visible;
} tile_t;

typedef struct tiles_init_data {
	int num_cols;
	int num_rows;
	int num_layers;
	int y_offset;
	int z_offset;
	int dstrect_size;
	int srcrect_size;
	int hitbox_size;
} tiles_init_data_t;

typedef struct tiles {
	tiles_init_data_t init_data;
	SDL_Texture *tex;
	int num_tiles;
	int num_tiles_per_layer;
	tile_t tiles[MAX_NUM_TILES];
} tiles_t;

int tiles_init(tiles_t *tiles, SDL_Renderer *ren, const char *path_to_bmp, tiles_init_data_t id);
int tiles_update(tiles_t *tiles, SDL_Point mouse, int is_left_click);
int tiles_draw(const tiles_t *tiles, SDL_Renderer *ren);
void tiles_del(tiles_t *tiles);

#endif
