#ifndef GROUND_H
#define GROUND_H

#include "game.h"
#include "blocks.h"

typedef struct ground ground_t;

ground_t *ground_new(blocks_init_data_t init_data, size_t tex_id);
const blocks_t *ground_get_blocks(const ground_t *ground);
int ground_update(ground_t *ground, SDL_Point mouse, int is_left_click);
void ground_del(ground_t *ground);

#endif
