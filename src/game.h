#ifndef GAME_H
#define GAME_H

#include "ground.h"
#include "entity.h"
#include "error.h"

typedef struct game_init_data {
	char *title;
	int win_w;
	int win_h;
	int vsync;
	ground_init_data_t ground_init_data;
	entity_init_data_t entity_init_data;
} game_init_data_t;

int game_init(game_init_data_t init_data);
int game_run();
void game_quit();

#endif
