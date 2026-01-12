#include "ground.h"
#include "error.h"
#include "debug.h"
#include <stdlib.h>

struct ground {
	ground_init_data_t init_data;
};

ground_t *ground_new(ground_init_data_t init_data) {
	ground_t *ground = calloc(1, sizeof(ground_t));
	if (!ground) {
		SET_ERR("Failed to allocate ground.");
		return NULL;
	}
	DBG("Ground allocated.");

	ground->init_data = init_data;

	return ground;
}

void ground_del(ground_t *ground) {
	if (ground) {
		free(ground);
		DBG("Ground destroyed.");
	}
}
