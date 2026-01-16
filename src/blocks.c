#include "blocks.h"
#include "error.h"
#include "debug.h"
#include <stdlib.h>

int blocks_init(
	blocks_t *blocks,
	blocks_init_data_t init_data,
	size_t tex_id
) {
	if (!blocks) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	const size_t num_layers = init_data.num_layers;
	const size_t num_rows = init_data.num_rows;
	const size_t num_cols = init_data.num_cols;
	const float block_size = init_data.block_size;
	const float hitbox_size = init_data.hitbox_size;
	const int img_size = init_data.img_size;
	const float origin_x = init_data.origin_x;
	const float origin_y = init_data.origin_y;
	const float origin_z = init_data.origin_z;
	const float x_offset = init_data.x_offset;
	const float y_offset = init_data.y_offset;
	const float z_offset = init_data.z_offset;

	blocks->blocks = calloc(num_layers, sizeof(block_t**));
	if (!blocks->blocks) {
		SET_ERR("Failed to allocate layers of blocks.");
		return 1;
	}
	DBG("Layers of blocks allocated.");

	for (size_t layer = 0; layer < num_layers; layer++) {
		blocks->blocks[layer] = calloc(num_rows, sizeof(block_t*));
		if (!blocks->blocks[layer]) {
			SET_ERR("Failed to allocate rows of blocks.");
			blocks_del(blocks);
			return 1;
		}
		DBG("Rows of blocks allocated.");
		for (size_t row = 0; row < num_rows; row++) {
			blocks->blocks[layer][row] =
				calloc(num_cols, sizeof(blocks_t));
			if (!blocks->blocks[layer][row]) {
				SET_ERR("Failed to allocated cols of blocks.");
				blocks_del(blocks);
				return 1;
			}
			DBG("Cols of blocks allocated.");
			for (size_t col = 0; col < num_cols; col++) {
				block_t block = {0};

				block.dstrect.x =
					origin_x + (float)col * x_offset -
					(float)row * y_offset;
				block.dstrect.y =
					origin_y + origin_z +
				       	(float)row * y_offset +
					(float)col * z_offset -
					(float)layer * z_offset;
				block.dstrect.w = block_size;
				block.dstrect.h = block_size;

				block.hitbox.x =
					block.dstrect.x +
					(block_size - hitbox_size) / 2;
				block.hitbox.y =
					block.dstrect.y + 1;
				block.hitbox.w = hitbox_size;
				block.hitbox.h = hitbox_size;

				block.srcrect.x = 0;
				block.srcrect.y = 0;
				block.srcrect.w = img_size;
				block.srcrect.h = img_size;

				block.is_active = 1;

				blocks->blocks[layer][row][col] = block;
			}
		}
	}

	blocks->tex_id = tex_id;
	blocks->init_data = init_data;

	return 0;
}

void blocks_del(blocks_t *blocks) {
	if (!blocks && blocks->blocks) return;
	const size_t num_layers = blocks->init_data.num_layers;
	const size_t num_rows = blocks->init_data.num_rows;
	for (size_t layer = 0; layer < num_layers; layer++) {
		for (size_t row = 0; row < num_rows; row++) {
			free(blocks->blocks[layer][row]);
			DBG("Cols of blocks freed.");
		}
		if (blocks->blocks[layer]) {
			free(blocks->blocks[layer]);
			DBG("Rows of blocks freed.");
		}
	}
	free(blocks->blocks);
	DBG("Layers of blocks freed.");
}
