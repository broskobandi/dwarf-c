#include "entity.h"
#include "error.h"
#include "debug.h"
#include <stdlib.h>

struct entity {
	entity_init_data_t init_data;
};

entity_t *entity_new(entity_init_data_t init_data) {
	entity_t *entity = calloc(1, sizeof(entity_t));
	if (!entity) {
		SET_ERR("Failed to allocate entity.");
		return NULL;
	}
	DBG("Entity allocated.");

	entity->init_data = init_data;

	return entity;
}

void entity_del(entity_t *entity) {
	if (entity) {
		free(entity);
		DBG("Entity destroyed.");
	}
}
