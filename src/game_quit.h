#ifndef GAME_QUIT_H
#define GAME_QUIT_H

#include "game_types.h"

void game_destroy_ren(game_t *game);
void game_destroy_win(game_t *game);
void game_terminate_sdl(game_t *game);

#endif
