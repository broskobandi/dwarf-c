#ifndef GAME_INIT_H
#define GAME_INIT_H

#include "game_types.h"
#include <stdint.h>

int game_init_sdl(game_t *game);
int game_init_win(game_t *game, const char *title, int w, int h);
int game_init_ren(game_t *game, int has_vsync);
int game_init_variables(game_t *game, uint8_t r, uint8_t g, uint8_t b);

#endif
