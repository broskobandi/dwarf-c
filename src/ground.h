#ifndef GROUND_H
#define GROUND_H

#include <stddef.h>

typedef struct ground_init_data {
	size_t num_cols;
	size_t num_rows;
	size_t num_layers;
	char *path_to_bmp;
	float block_size;
	float hitbox_size;
	float x_offset;
	float y_offset;
	float z_offset;
} ground_init_data_t;

typedef struct ground ground_t;

ground_t *ground_new(ground_init_data_t init_data);
void ground_del(ground_t *ground);

#endif
