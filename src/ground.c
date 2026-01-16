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
	const block_t *selected_block;
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

const block_t *ground_get_selected_block(const ground_t *ground) {
	if (!ground) {
		SET_ERR("Invalid argument.");
		return NULL;
	}

	return ground->selected_block;
}

int ground_update(
	ground_t *ground,
	SDL_Point mouse,
	int is_left_click,
	int is_right_click
) {
	if (!ground) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	const SDL_FPoint fmouse = {
		.x = (float)mouse.x,
		.y = (float)mouse.y
	};

	const size_t num_layers = ground->blocks.init_data.num_layers;
	const size_t num_rows = ground->blocks.init_data.num_rows;
	const size_t num_cols = ground->blocks.init_data.num_cols;
	const float hitbox_size = ground->blocks.init_data.hitbox_size;
	block_t ***blocks = ground->blocks.blocks;

	for (size_t layer = 0; layer < num_layers; layer++) {
		for (size_t row = 0; row < num_rows; row++) {
			for (size_t col = 0; col < num_cols; col++) {
				block_t *block = &blocks[layer][row][col];

				int is_blocked_from_above = 0;
				int is_blocked_from_right = 0;
				int is_blocked_from_left = 0;
				int is_blocked_from_right_up = 0;
				int is_blocked_from_left_up = 0;
				int is_blocked_from_right_down = 0;
				int is_blocked_from_left_down = 0;
				int is_blocked_from_above_right_up = 0;

				// Set blocked flags

				if (	layer + 1 >= num_layers ||
					!blocks[layer + 1][row][col].is_active
				) {
					is_blocked_from_above = 0;
				} else {
					is_blocked_from_above = 1;
				}

				if (	row - 1 >= 0 &&
					row - 1 < num_rows &&
					col + 1 < num_cols &&
					blocks[layer][row-1][col+1].is_active
				) {
					is_blocked_from_right = 1;
				} else {
					is_blocked_from_right = 0;
				}

				if (	row + 1 < num_rows &&
					col - 1 >= 0 &&
					col - 1 < num_cols &&
					blocks[layer][row+1][col-1].is_active
				) {
					is_blocked_from_left = 1;
				} else {
					is_blocked_from_left = 0;
				}

				if (	row - 1 >= 0 &&
					row - 1 < num_rows &&
					blocks[layer][row-1][col].is_active
				) {
					is_blocked_from_right_up = 1;
				} else {
					is_blocked_from_right_up = 0;
				}

				if (	col + 1 < num_cols &&
					blocks[layer][row][col+1].is_active
				) {
					is_blocked_from_right_down = 1;
				} else {
					is_blocked_from_right_down = 0;
				}

				if (	col - 1 >= 0 &&
					col - 1 < num_cols &&
					blocks[layer][row][col-1].is_active
				) {
					is_blocked_from_left_up = 1;
				} else {
					is_blocked_from_left_up = 0;
				}

				if (	row + 1 < num_rows &&
					blocks[layer][row+1][col].is_active
				) {
					is_blocked_from_left_down = 1;
				} else {
					is_blocked_from_left_down = 0;
				}

				if (	layer + 1 < num_layers &&
					row - 1 >= 0 &&
					row - 1 < num_rows &&
					blocks[layer+1][row-1][col].is_active
				) {
					is_blocked_from_above_right_up = 1;
				} else {
					is_blocked_from_above_right_up = 0;
				}

				// Update highlighted flag

				if (	fmouse.x >= block->hitbox.x &&
				    	fmouse.x <= block->hitbox.x +
						hitbox_size &&
				    	fmouse.y >= block->hitbox.y &&
				    	fmouse.y <= block->hitbox.y +
						hitbox_size &&
					!is_blocked_from_above
				) {
					block->is_highlighted = 1;
				} else {
					block->is_highlighted = 0;
				}

				// Update active flag

				if (	block->is_highlighted &&
					is_right_click
				) {
					block->is_active = 0;
				}

				// Update selected block

				if (	block->is_highlighted &&
					is_left_click
				) {
					ground->selected_block = block;
				}

				// Update shadow flags
				
				if (	is_blocked_from_right &&
					!is_blocked_from_right_down &&
					block->is_active
				) {
					block->has_right_shadow = 1;
				} else {
					block->has_right_shadow = 0;
				}

				if (	!is_blocked_from_left_down &&
					!is_blocked_from_left_up &&
					block->is_active
				) {
					block->has_left_shadow = 1;
				} else {
					block->has_left_shadow = 0;
				}

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
