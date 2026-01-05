#include "dwarf.h"
#include "debug.h"
#include "error.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

int dwarf_init(dwarf_t *dwarf, SDL_Renderer *ren, const char *path_to_bmp, dwarf_init_data_t id) {
	if (!dwarf || !ren || !path_to_bmp) {
		SET_ERR("Arguments cannot be NULL.");
		return 1;
	}

	SDL_Surface *sur = SDL_LoadBMP(path_to_bmp);
	if (!sur) {
		SET_ERR("Failed to load dwarf bmp.");
		return 1;
	}
	DBG("Dwarf bmp loaded.");

	dwarf->tex = SDL_CreateTextureFromSurface(ren, sur);
	if (!dwarf->tex) {
		SET_ERR("Failed to create dwarf texture.");
		SDL_FreeSurface(sur);
		DBG("Dwarf surface freed.");
		return 1;
	}
	DBG("Dwarf texture created.");

	SDL_FreeSurface(sur);
	DBG("Dwarf surface freed.");

	dwarf->init_data = id;
	dwarf->dstrect.x = id.start_pos.x;
	dwarf->dstrect.y = id.start_pos.y;
	dwarf->dstrect.w = id.dstrect_size;
	dwarf->dstrect.h = id.dstrect_size;
	dwarf->srcrect.x = 0;
	dwarf->srcrect.y = 0;
	dwarf->srcrect.w = id.srcrect_size;
	dwarf->srcrect.h = id.srcrect_size;
	dwarf->hitbox.x = dwarf->dstrect.x;
	dwarf->hitbox.y = dwarf->dstrect.y;
	dwarf->hitbox.w = id.hitbox_size;
	dwarf->hitbox.h = id.hitbox_size;
	dwarf->time_of_last_anim_update = 0;
	dwarf->cur_img = 0;
	dwarf->has_target = false;
	dwarf->is_moving = false;

	return 0;
}

void dwarf_del(dwarf_t *dwarf) {
	if (dwarf) {
		if (dwarf->tex) {
			SDL_DestroyTexture(dwarf->tex);
			DBG("Dwarf texture destryoed.");
		}
	}
}

int dwarf_update(
	dwarf_t *dwarf,
	const tiles_t *tiles,
	Uint32 time,
	SDL_Point mouse_pos,
	bool is_left_click
) {
	if (!dwarf || !dwarf->tex) {
		SET_ERR("Dwarf cannot be NULL.");
		return 1;
	}

	const Uint32 ticks_per_anim_update = dwarf->init_data.ticks_per_anim_update;
	const int num_imgs = dwarf->init_data.num_imgs;
	const bool is_moving = dwarf->is_moving;

	// if (is_left_click) {
	// 	dwarf->target.x = mouse_pos
	// }

	if (time - dwarf->time_of_last_anim_update >= ticks_per_anim_update && is_moving) {
		dwarf->time_of_last_anim_update = time;
		if (dwarf->cur_img + 1 < num_imgs) {
			dwarf->cur_img++;
		} else {
			dwarf->cur_img = 0;
		}
	}

	return 0;
}

int dwarf_draw(const dwarf_t *dwarf, SDL_Renderer *ren) {
	if (!dwarf || !dwarf->tex || !ren) {
		SET_ERR("Arguments cannot be NULL.");
		return 1;
	}

	return 0;
}
