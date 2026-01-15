#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include <stdint.h>

typedef struct game_init_data {
	const char *title;
	int win_w;
	int win_h;
	int has_vsync;
	uint8_t bg_r;
	uint8_t bg_g;
	uint8_t bg_b;
} game_init_data_t;

int game_init(game_init_data_t game_init_data);
int game_run();
const char *game_get_error();
void game_quit();

#endif
