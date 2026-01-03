#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL_events.h>
typedef struct sdl sdl_t;

sdl_t *sdl_new(const char *title, int w, int h);
void sdl_del(sdl_t *sdl);

int sdl_poll_event(sdl_t *sdl);
int sdl_has_event_type(const sdl_t *sdl, SDL_EventType type);
int sdl_has_keycode(const sdl_t *sdl, SDL_Keycode keycode);
int sdl_set_draw_color(const sdl_t *sdl, SDL_Color color);
int sdl_render_clear(const sdl_t *sdl);
int sdl_render_present(const sdl_t *sdl);

#endif
