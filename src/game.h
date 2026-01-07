#ifndef GAME_H
#define GAME_H

#include "init.h"

typedef struct game game_t;

game_t *game_init(
	game_init_data_t game_init_data,
	ground_init_data_t ground_init_data
);
int game_run(game_t *game);
void game_del(game_t *game);

#endif
