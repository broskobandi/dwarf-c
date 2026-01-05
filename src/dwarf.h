#ifndef DWARF_H
#define DWARF_H

#include "tiles.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>

typedef struct dwarf_init_data {
	SDL_Point start_pos;
	int dstrect_size;
	int srcrect_size;
	int hitbox_size;
	int num_imgs;
	Uint32 ticks_per_anim_update;
	int pixels_per_frame;
} dwarf_init_data_t;

typedef struct dwarf {
	SDL_Rect dstrect;
	SDL_Rect hitbox;
	SDL_Rect srcrect;
	Uint32 time_of_last_anim_update;
	SDL_Texture *tex;
	dwarf_init_data_t init_data;
	int cur_img;
	SDL_Point target;
	bool has_target;
	bool is_moving;
} dwarf_t;

int dwarf_init(dwarf_t *dwarf, SDL_Renderer *ren, const char *path_to_bmp, dwarf_init_data_t init_data);
void dwarf_del(dwarf_t *dwarf);
int dwarf_update(
	dwarf_t *dwarf,
	const tiles_t *tiles,
	Uint32 time,
	SDL_Point mouse_pos,
	bool is_left_click
);
int dwarf_draw(const dwarf_t *dwarf, SDL_Renderer *ren);

#endif
