#ifndef ENTITY_H
#define ENTITY_H

#include "init.h"
#include <stddef.h>
#include <SDL2/SDL_rect.h>

typedef struct entity_render_data  {
	SDL_FRect dstrect;
	SDL_Rect srcrect;
	size_t tex_id;
} entity_render_data_t;

typedef struct entity entity_t;

entity_t *entity_new(entity_init_data_t init_data, size_t tex_id);
const entity_render_data_t *entity_get_render_data(const entity_t* entity);
void entity_del(entity_t *entity);

#endif
