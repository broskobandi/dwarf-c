#ifndef ENTITY_H
#define ENTITY_H

typedef struct entity_init_data {
	float size;
	char *path_to_bmp;
	float hitbox_size;
} entity_init_data_t;

typedef struct entity entity_t;

entity_t *entity_new(entity_init_data_t init_data);
void entity_del(entity_t *entity);

#endif
