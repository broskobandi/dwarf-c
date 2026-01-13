#ifndef GROUND_H
#define GROUND_H

#include <SDL2/SDL_rect.h>

#define ROWS_BUFF_SIZE 128
#define COLS_BUFF_SIZE 128
#define LAYERS_BUFF_SIZE 16

#include <stddef.h>

/** The ground init data. */
typedef struct ground_init_data {
	size_t num_cols;
	size_t num_rows;
	size_t num_layers;
	char *path_to_bmp;
	float block_size;
	int img_size;
	float hitbox_size;
	float x_offset;
	float y_offset;
	float z_offset;
	float origin_x;
	float origin_y;
	float origin_z;
	size_t right_shadow_img_index;
	size_t left_shadow_img_index;
	size_t top_shadow_img_index;
	size_t right_light_img_index;
	size_t front_corner_img_index;
	size_t highlight_img_index;
	size_t select_img_index;
} ground_init_data_t;

/** The block object that the ground is built up of. */
typedef struct block {
	SDL_FRect hitbox;
	SDL_FRect dstrect;
	SDL_Rect srcrect;
	size_t tex_id;
	int has_top_shadow;
	int has_left_shadow;
	int has_right_shadow;
	int has_right_light;
	int is_highlighted;
	int is_selected;
	int is_visible;
	int is_active;
} block_t;

/** Vector object for managing the 3d array of blocks. */
typedef struct blocks_vec {
	block_t ***blocks;
	ground_init_data_t init_data;
	block_t *selected_block;
} blocks_vec_t;

/** Opaque handle for the ground object. */
typedef struct ground ground_t;

/** Creates a new ground instance.
 * \param init_data The ground init data.
 * \param tex_id The id of the texture.
 * \returns A pointer to the new ground object or NULL on failure.
 * Sets the internal error state on failure. */
ground_t *ground_new(ground_init_data_t init_data, size_t tex_id);

/** Updates the internal state of the gound object.
 * \param ground A pointer to the ground object.
 * \param mouse An SDL_FPoint containing the current mouse pos.
 * \param left_click A boolean-like int representing the left mouse 
 * button state.
 * \returns 0 success or 1 on failure.
 * Sets the internal error state on failure. */
int ground_update(ground_t *ground, SDL_FPoint mouse, int left_click, int right_click);

/** Returns a const pointer to the block vector assoicated with a ground
 * object.
 * \param ground A pointer to the ground object.
 * \returns A const pointer to the vector object or NULL on failure. 
 * Sets the internal error state on failure. */
const blocks_vec_t *ground_get_blocks_vec(const ground_t *ground);

/** Cleans up all memory allocated for the ground object.
 * \param ground A pointer to the ground object. */
void ground_del(ground_t *ground);

#endif
