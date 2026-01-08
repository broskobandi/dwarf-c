#include "ground.h"
#include "debug.h"
#include "error.h"
#include <SDL2/SDL_rect.h>
#include <stdio.h>
#include <stdlib.h>

static bool g_is_ground_init = false;

struct ground {
	ground_render_data_t render_data;
	ground_init_data_t init_data;
	size_t tex_id;
};

ground_t *ground_init(ground_init_data_t init_data, size_t tex_id) {
	if (g_is_ground_init) {
		SET_ERR("Ground has already been initialized.");
		return NULL;
	}

	ground_t *ground = calloc(1, sizeof(ground_t));
	if (!ground) {
		SET_ERR("Failed to allocate ground.");
		return NULL;
	}
	DBG("Ground allocated.");

	ground->init_data = init_data;
	ground->render_data.tex_id = tex_id;

	const size_t num_rows = ground->init_data.num_rows;
	const size_t num_cols = ground->init_data.num_cols;
	const size_t num_layers = ground->init_data.num_layers;
	const float origin_x = ground->init_data.origin_x;
	const float origin_y = ground->init_data.origin_y;
	const float x_offset = ground->init_data.x_offset;
	const float y_offset = ground->init_data.y_offset;
	const float z_offset = ground->init_data.z_offset;
	const float dstrect_size = ground->init_data.in_game_sprite_size;
	const float hitbox_size = ground->init_data.hitbox_size;
	const int srcrect_size = ground->init_data.real_img_size;

	ground_render_data_t *render_data = &ground->render_data;

	for (size_t layer = 0; layer < num_layers; layer++) {
	 for (size_t row = 0; row < num_rows; row++) {
	  for (size_t col = 0; col < num_cols; col++) {
		if (render_data->num_blocks + 1 > MAX_NUM_BLOCKS) {
			SET_ERR("block array overflow.");
			return NULL;
		}
		SDL_FRect dstrect = {
			.x = origin_x + (float)col * x_offset - (float)row * x_offset,
			.y = origin_y + (float)row * y_offset + (float)col * z_offset -
				 (float)layer * z_offset,
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
		render_data->blocks[render_data->num_blocks] = block;
		render_data->num_blocks++;
	  }
	 }
	}

	g_is_ground_init = true;
	DBG("Ground initialized.");

	return ground;
}

const ground_render_data_t *ground_get_render_data(const ground_t *ground) {
	if (!ground || !g_is_ground_init) {
		SET_ERR("Invalid argument.");
		return NULL;
	}

	return &ground->render_data;
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
