#include "ground.h"
#include "error.h"
#include "debug.h"
#include <stdlib.h>
#include <stddef.h>

struct ground {
	blocks_vec_t blocks_vec;
	ground_init_data_t init_data;
};

ground_t *ground_new(ground_init_data_t init_data, size_t tex_id) {
	ground_t *ground = calloc(1, sizeof(ground_t));
	if (!ground) {
		SET_ERR("Failed to allocate ground.");
		return NULL;
	}
	DBG("Ground allocated.");

	const size_t num_cols = init_data.num_cols;
	const size_t num_rows = init_data.num_rows;
	const size_t num_layers = init_data.num_layers;
	const float dstrect_size = init_data.block_size;
	const int srcrect_size = init_data.img_size;
	const float x_offset = init_data.x_offset;
	const float y_offset = init_data.y_offset;
	const float z_offset = init_data.z_offset;
	const float origin_x = init_data.origin_x;
	const float origin_y = init_data.origin_y;
	const float origin_z = init_data.origin_z;

	block_t ****blocks = &ground->blocks_vec.blocks;
	ground->blocks_vec.num_layers = num_layers;
	ground->blocks_vec.num_rows = num_rows;
	ground->blocks_vec.num_cols = num_cols;

	*blocks = calloc(num_layers, sizeof(block_t **));
	if (!*blocks) {
		SET_ERR("Failed to allocate blocks layers.");
		return NULL;
	}
	for (size_t layer = 0; layer < num_layers; layer++) {
		(*blocks)[layer] = calloc(num_rows, sizeof(block_t *));
		if (!(*blocks)[layer]) {
			SET_ERR("Failed to allocate blocks rows.");
			return NULL;
		}
		for (size_t row = 0; row < num_rows; row++) {
			(*blocks)[layer][row] = calloc(num_cols, sizeof(block_t));
			if (!(*blocks)[layer][row]) {
				SET_ERR("Failed to allocate blocks cols.");
				return NULL;
			}
			for (size_t col = 0; col < num_cols; col++) {
				block_t *block = &(*blocks)[layer][row][col];
				block->tex_id = tex_id;
				block->srcrect.x = 0;
				block->srcrect.y = 0;
				block->srcrect.w = srcrect_size;
				block->srcrect.h = srcrect_size;
				block->dstrect.x =
					origin_x +
					(float)col * x_offset -
					(float)row * x_offset;
				block->dstrect.y =
					origin_y +
					(float)row * y_offset +
					(float)col * y_offset -
					(float)layer * z_offset;
				block->dstrect.w = dstrect_size;
				block->dstrect.h = dstrect_size;
			}
		}
	}
	DBG("Blocks created.");


	ground->init_data = init_data;

	return ground;
}

int ground_update(ground_t *ground) {
	if (!ground) {
		SET_ERR("Invalid arguments.");
		return 1;
	}

	

	return 0;
}

const blocks_vec_t *ground_get_blocks_vec(const ground_t *ground) {
	if (!ground) {
		SET_ERR("Invalid arguments.");
		return NULL;
	}

	return (const blocks_vec_t*)&ground->blocks_vec;
}

void ground_del(ground_t *ground) {
	if (ground) {
		if (ground->blocks_vec.blocks) {
		 for (size_t layer = 0; layer < ground->blocks_vec.num_layers; layer++) {
		  if (ground->blocks_vec.blocks[layer]) {
		   for (size_t row = 0; row < ground->blocks_vec.num_rows; row++) {
		    if (ground->blocks_vec.blocks[layer][row]) {
		     free(ground->blocks_vec.blocks[layer][row]);
		    }
		   }
		  }
		  free(ground->blocks_vec.blocks[layer]);
		 }
		 free(ground->blocks_vec.blocks);
		}
		free(ground);
		DBG("Ground destroyed.");
	}
}
