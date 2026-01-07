#include "ground.h"
#include "debug.h"
#include "error.h"
#include <SDL2/SDL_rect.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_BLOCKS 1024LU

static bool g_is_ground_init = false;

struct ground {
	block_t blocks[MAX_NUM_BLOCKS];
	size_t num_blocks;
	ground_init_data_t init_data;
	size_t tex_id;
};

ground_t *ground_init(ground_init_data_t init_data, size_t tex_id) {
	if (g_is_ground_init) {
		SET_ERR("Ground has already been initialized.");
		return NULL;
	}

	ground_t *g = calloc(1, sizeof(ground_t));
	if (!g) {
		SET_ERR("Failed to allocate ground.");
		return NULL;
	}
	DBG("Ground allocated.");

	g->init_data = init_data;
	g->tex_id = tex_id;

	const size_t num_rows = g->init_data.num_rows;
	const size_t num_cols = g->init_data.num_cols;
	const size_t num_layers = g->init_data.num_layers;
	const float origin_x = g->init_data.origin_x;
	const float origin_y = g->init_data.origin_y;
	const float x_offset = g->init_data.x_offset;
	const float y_offset = g->init_data.y_offset;
	const float z_offset = g->init_data.z_offset;
	const float dstrect_size = g->init_data.in_game_sprite_size;
	const float hitbox_size = g->init_data.hitbox_size;
	const int srcrect_size = g->init_data.real_img_size;

	for (size_t layer = 0; layer < num_layers; layer++) {
	 for (size_t row = 0; row < num_rows; row++) {
	  for (size_t col = 0; col < num_cols; col++) {
		if (g->num_blocks + 1 > MAX_NUM_BLOCKS) {
			SET_ERR("MAX_NUM_BLOCKS buffer overflow.");
			return NULL;
		}
		SDL_FRect dstrect = {
			.x = origin_x + (float)col * x_offset,
			.y = origin_y + (float)col * y_offset -
				z_offset * (float)layer,
			.w = dstrect_size,
			.h = dstrect_size
		};
		SDL_FRect hitbox = {
			.x = dstrect.x + (dstrect_size - hitbox_size) / 2,
			.y = dstrect.y,
			.w = hitbox_size,
			.h = hitbox_size
		};
		SDL_Rect srcrect = {
			.x = 0,
			.y = 0,
			.w = srcrect_size,
			.h = srcrect_size
		};
		block_t block = {
			.dstrect = dstrect,
			.srcrect = srcrect,
			.hitbox = hitbox,
			.layer = layer,
			.row = row,
			.col = col
		};
		g->blocks[g->num_blocks] = block;
		g->num_blocks++;
	  }
	 }
	}

	g_is_ground_init = true;
	DBG("Ground initialized.");

	return g;
}

const block_t *ground_get_blocks(const ground_t *ground) {
	if (!ground || !g_is_ground_init) {
		SET_ERR("Invalid argument.");
		return NULL;
	}

	return ground->blocks;
}

size_t ground_get_num_blocks(const ground_t *ground) {
	if (!ground || !g_is_ground_init) {
		SET_ERR("Invalid argument.");
		return -1lu;
	}

	return ground->num_blocks;
}

size_t ground_get_tex_id(const ground_t *ground) {
	if (!ground || !g_is_ground_init) {
		SET_ERR("Invalid argument.");
		return -1lu;
	}

	return ground->tex_id;
}

int ground_update(ground_t *g) {
	if (!g) {
		SET_ERR("Invalid argument.");
		return 1;
	}


	return 0;
}

void ground_del(ground_t *ground) {
	if (ground) {
		free(ground);
		g_is_ground_init = false;
		DBG("Ground destroyed.");
	}
}
