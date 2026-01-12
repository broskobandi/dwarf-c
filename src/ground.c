#include "ground.h"
#include "error.h"
#include "debug.h"
#include <stdlib.h>
#include <stddef.h>

#define ROWS_BUFF_SIZE 128
#define COLS_BUFF_SIZE 128
#define LAYERS_BUFF_SIZE 16

struct ground {
	ground_init_data_t init_data;
	size_t tex_id;
};

ground_t *ground_new(ground_init_data_t init_data, size_t tex_id) {
	ground_t *ground = calloc(1, sizeof(ground_t));
	if (!ground) {
		SET_ERR("Failed to allocate ground.");
		return NULL;
	}
	DBG("Ground allocated.");

	ground->init_data = init_data;
	ground->tex_id = tex_id;



	return ground;
}

void ground_del(ground_t *ground) {
	if (ground) {
		free(ground);
		DBG("Ground destroyed.");
	}
}
