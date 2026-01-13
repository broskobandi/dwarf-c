#include "ground.h"
#include "error.h"
#include "debug.h"
#include <stdlib.h>
#include <stddef.h>
#include <SDL2/SDL_rect.h>

/** The definition of the opaque ground object. */
struct ground {
	blocks_vec_t blocks_vec;
	ground_init_data_t init_data;
};

/** Creates a new ground instance.
 * \param init_data The ground init data.
 * \param tex_id The id of the texture.
 * \returns A pointer to the new ground object or NULL on failure.
 * Sets the internal error state on failure. */
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
	const float hitbox_size = init_data.hitbox_size;
	const float x_offset = init_data.x_offset;
	const float y_offset = init_data.y_offset;
	const float z_offset = init_data.z_offset;
	const float origin_x = init_data.origin_x;
	const float origin_y = init_data.origin_y;
	const float origin_z = init_data.origin_z;

	block_t ****blocks = &ground->blocks_vec.blocks;
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
					origin_z +
					origin_y +
					(float)row * y_offset +
					(float)col * y_offset -
					(float)layer * z_offset;
				block->dstrect.w = dstrect_size;
				block->dstrect.h = dstrect_size;
				block->hitbox.x =
					block->dstrect.x +
					(dstrect_size - hitbox_size) / 2;
				block->hitbox.y = block->dstrect.y;
				block->hitbox.w = hitbox_size;
				block->hitbox.h = hitbox_size;
				block->is_active = 1;
			}
		}
	}
	DBG("Blocks created.");


	ground->blocks_vec.init_data = init_data;

	return ground;
}

/** Updates the internal state of the gound object.
 * \param ground A pointer to the ground object.
 * \param mouse An SDL_FPoint containing the current mouse pos.
 * \param left_click A boolean-like int representing the left mouse 
 * button state.
 * \returns 0 success or 1 on failure.
 * Sets the internal error state on failure. */
int ground_update(ground_t *ground, SDL_FPoint mouse, int left_click, int right_click) {
	if (!ground) {
		SET_ERR("Invalid arguments.");
		return 1;
	}

	blocks_vec_t *blocks_vec = &ground->blocks_vec;
	block_t ***blocks = blocks_vec->blocks;
	const ground_init_data_t *init_data = &blocks_vec->init_data;
	const size_t num_layers = init_data->num_layers;
	const size_t num_cols = init_data->num_cols;
	const size_t num_rows = init_data->num_rows;

	for (size_t layer = 0; layer < num_layers; layer++) {
		for (size_t row = 0; row < num_rows; row++) {
			for (size_t col = 0; col < num_cols; col++) {
				block_t *block = &blocks[layer][row][col];
				const SDL_FRect hitbox = block->hitbox;

				int is_blocked_from_above = 0;
				int is_blocked_from_right_up = 0;
				int is_blocked_from_right_down = 0;
				/* int is_blocked_from_left_up = 0; */
				/* int is_blocked_from_left_down = 0; */
				/* int is_blocked_from_right = 0; */
				/* int is_blocked_from_left = 0; */
				/* int is_blocked_from_above_right_up = 0; */

				if (	layer + 1 < num_layers &&
					blocks[layer + 1][row][col].is_active
				) {
					is_blocked_from_above = 1;
				} else {
					is_blocked_from_above = 0;
				}

				if ( 	row - 1 >= 0 &&
					row - 1 < num_rows
					/* blocks[layer][ */
				) {
					is_blocked_from_right_up = 1;
				} else {
					is_blocked_from_right_up = 0;
				}

				/* if (	 */

				if ( 	!is_blocked_from_above &&
					block->is_active &&
					mouse.x >= hitbox.x &&
					mouse.x <= hitbox.x + hitbox.w &&
					mouse.y >= hitbox.y &&
					mouse.y <= hitbox.y + hitbox.h
				) {
					block->is_highlighted = 1;
				} else {
					block->is_highlighted = 0;
				}

				if (	block->is_highlighted &&
					left_click 
				) {
					blocks_vec->selected_block = block;
				}

				if (	block->is_highlighted &&
					right_click 
				) {
					block->is_active = 0;
				}

				if (block->is_active) {
					block->is_visible = 1;
				} else {
					block->is_visible = 0;
				}
			}
		}
	}

	return 0;
}

/** Returns a const pointer to the block vector assoicated with a ground
 * object.
 * \param ground A pointer to the ground object.
 * \returns A const pointer to the vector object or NULL on failure. 
 * Sets the internal error state on failure. */
const blocks_vec_t *ground_get_blocks_vec(const ground_t *ground) {
	if (!ground) {
		SET_ERR("Invalid arguments.");
		return NULL;
	}

	return (const blocks_vec_t*)&ground->blocks_vec;
}

/** Cleans up all memory allocated for the ground object.
 * \param ground A pointer to the ground object. */
void ground_del(ground_t *ground) {
	if (!ground) return;
	const size_t num_layers = ground->blocks_vec.init_data.num_layers;
	const size_t num_rows = ground->blocks_vec.init_data.num_rows;
	if (ground->blocks_vec.blocks) {
		for (size_t layer = 0; layer < num_layers; layer++) {
			if (!ground->blocks_vec.blocks[layer]) continue;
			for (size_t row = 0; row < num_rows; row++) {
				if (!ground->blocks_vec.blocks[layer][row]) continue;
				free(ground->blocks_vec.blocks[layer][row]);
			}
			free(ground->blocks_vec.blocks[layer]);
		}
		free(ground->blocks_vec.blocks);
	}
	free(ground);
	DBG("Ground destroyed.");
}
