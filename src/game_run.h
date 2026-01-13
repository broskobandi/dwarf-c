#ifndef GAME_RUN
#define GAME_RUN

#include "game_types.h"
#include <SDL2/SDL_pixels.h>

int game_poll_events(game_t *game);
int game_clear(game_t *game);
int game_set_draw_color(game_t *game, SDL_Color col);
int game_present(game_t *game);

#endif
