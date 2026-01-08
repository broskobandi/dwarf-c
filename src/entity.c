#include "entity.h"
#include "error.h"
#include "debug.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

struct entity {
	entity_init_data_t init_data;
	float x, y, z; // top left corner
	SDL_FRect hitbox;
	entity_render_data_t render_data;
	size_t cur_img;
};

entity_t *entity_new(entity_init_data_t init_data, size_t tex_id) {
	entity_t *entity = calloc(1, sizeof(entity_t));
	if (!entity) {
		SET_ERR("Failed to allocate entity.");
		return NULL;
	}
	DBG("Entity allocated.");

	entity->init_data = init_data;
	entity->render_data.tex_id = tex_id;
	entity->x = init_data.x;
	entity->y = init_data.y;
	entity->z = init_data.z;
	entity->render_data.dstrect.x = init_data.x;
	entity->render_data.dstrect.y = init_data.y;
	entity->render_data.dstrect.w = init_data.in_game_sprite_size;
	entity->render_data.dstrect.h = init_data.in_game_sprite_size;
	entity->hitbox = entity->render_data.dstrect;
	entity->render_data.srcrect.x = 0;
	entity->render_data.srcrect.y = 0;
	entity->render_data.srcrect.h = init_data.real_img_size;
	entity->render_data.srcrect.w = init_data.real_img_size;

	return entity;
}

const entity_render_data_t *entity_get_render_data(const entity_t* entity) {
	if (!entity) {
		SET_ERR("Invalid argument.");
		return NULL;
	}

	return &entity->render_data;
}

void entity_del(entity_t *entity) {
	if (entity) {
		free(entity);
		DBG("Entity destroyed.");
	}
}
