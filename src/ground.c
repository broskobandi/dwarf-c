#include "ground.h"
#include "blocks.h"
#include "game.h"
#include "error.h"
#include "debug.h"
#include <stdlib.h>
#include <stddef.h>
#include <SDL2/SDL_rect.h>

struct ground {
	blocks_t blocks;
};

ground_t *ground_new(blocks_init_data_t init_data, size_t tex_id) {
	ground_t *ground = calloc(1, sizeof(ground_t));
	if (!ground) {
		SET_ERR("Failed to allocate ground.");
		return NULL;
	}
	DBG("Ground allocated.");

	if (blocks_init(&ground->blocks, init_data, tex_id)) {
		ground_del(ground);
		return NULL;
	}

	return ground;
}

const blocks_t *ground_get_blocks(const ground_t *ground) {
	if (!ground) {
		SET_ERR("Invalid argument.");
		return NULL;
	}

	return &ground->blocks;
}

int ground_update(ground_t *ground, SDL_Point mouse, int is_left_click) {
	if (!ground) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	const SDL_FPoint fmouse = {
		.x = (float)mouse.x,
		.y = (float)mouse.y
	};

	blocks_t *blocks = &ground->blocks;
	const size_t num_layers = blocks->init_data.num_layers;
	const size_t num_rows = blocks->init_data.num_rows;
	const size_t num_cols = blocks->init_data.num_cols;

	for (size_t layer = 0; layer < num_layers; layer++) {
		for (size_t row = 0; row < num_rows; row++) {
			for (size_t col = 0; col < num_cols; col++) {
				/* block_t *block = &blocks->blocks[layer][row][col]; */
			}
		}
	}

	return 0;
}

void ground_del(ground_t *ground) {
	if (ground) {
		blocks_del(&ground->blocks);
		free(ground);
		DBG("Ground destroyed.");
	}
}
