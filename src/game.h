#ifndef GAME_H
#define GAME_H

#include "ground.h"
#include "entity.h"
#include "error.h"

/** Game init data struct. */
typedef struct game_init_data {
	char *title;
	int win_w;
	int win_h;
	int vsync;
	ground_init_data_t ground_init_data;
	entity_init_data_t entity_init_data;
} game_init_data_t;

/** Initializes the game instance.
 * \param init_data The game init data.
 * \returns 0 on success or 1 on failure.
 * Sets the internal error state on failure. */
int game_init(game_init_data_t init_data);

/** Runs the game instance. 
 * \returns 0 on success or 1 on failure.
 * Sets the internal error state on failure. */
int game_run();

/** Cleans up all memory allocated for the game instance. */
void game_quit();

#endif
