#include "tiles.h"
#include "debug.h"
#include "error.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

int tiles_init(tiles_t *tiles, SDL_Renderer *ren, const char *path_to_bmp, tiles_init_data_t id) {
	if (!tiles || !ren || !path_to_bmp) {
		SET_ERR("Arguments cannot be NULL.");
		return 1;
	}

	tiles->init_data = id;
	const int num_layers = id.num_layers;
	const int num_rows = id.num_rows;
	const int num_cols = id.num_cols;
	const int dstrect_size = id.dstrect_size;
	const int srcrect_size = id.srcrect_size;
	const int hitbox_size = id.hitbox_size;
	const int y_offset = id.y_offset;
	const int z_offset = id.z_offset;

	SDL_Surface *sur = SDL_LoadBMP(path_to_bmp);
	if (!sur) {
		SET_ERR("Failed to load bmp.");
		return 1;
	}
	DBG("Surface loaded for tiles.");

	tiles->tex = SDL_CreateTextureFromSurface(ren, sur);
	if (!tiles->tex) {
		SET_ERR("Failed to create texture.");
		SDL_FreeSurface(sur);
		DBG("Surface freed for tiles.");
		return 1;
	}
	DBG("Texture created.");

	SDL_FreeSurface(sur);
	DBG("Surface freed for tiles.");

	tiles->num_tiles_per_layer = num_cols * num_rows;
	tiles->num_tiles = 0;

	for (int z = 0; z < num_layers; z++) {
		for (int y = 0; y < num_rows; y++) {
			int x_offset = y % 2 == 0 ? 0 : dstrect_size / 2;
			for (int x = 0; x < num_cols; x++) {
				if (tiles->num_tiles + 1 > MAX_NUM_TILES) {
					SET_ERR("Tiles buffer overflow.");
					tiles_del(tiles);
					return 1;
				}
				const SDL_Rect dstrect = {
					x * dstrect_size + x_offset,
					y * y_offset - z * z_offset,
					dstrect_size,
					dstrect_size
				};
				const SDL_Rect srcrect = {0, 0, srcrect_size, srcrect_size};
				const SDL_Rect hitbox = {
					dstrect.x + (dstrect_size - hitbox_size) / 2,
					dstrect.y,
					hitbox_size,
					hitbox_size
				};
				const bool is_staggered = x_offset ? true : false;
				tile_t tile = {0};
				tile.dstrect = dstrect;
				tile.srcrect = srcrect;
				tile.hitbox = hitbox;
				tile.is_staggered = is_staggered;
				tile.is_active = true;
				tiles->tiles[tiles->num_tiles] = tile;
				tiles->num_tiles++;
			}
		}
	}

	printf("%d\n", tiles->num_tiles);

	return 0;
}

void tiles_del(tiles_t *tiles) {
	if (tiles) {
		if (tiles->tex) {
			SDL_DestroyTexture(tiles->tex);
			DBG("Texture destroyed.");
		}
	}
}

int tiles_update(tiles_t *tiles_object, SDL_Point mouse, int is_left_click) {
	if (!tiles_object) {
		SET_ERR("Tiles cannot be NULL.");
		return 1;
	}
	const int num_tiles_per_layer = tiles_object->num_tiles_per_layer;
	const int num_tiles = tiles_object->num_tiles;
	const int hitbox_size = tiles_object->init_data.hitbox_size;
	const int dstrect_size = tiles_object->init_data.dstrect_size;
	const int num_cols = tiles_object->init_data.num_cols;
	const int num_rows = tiles_object->init_data.num_rows;
	const int y_offset = tiles_object->init_data.y_offset;
	const int z_offset = tiles_object->init_data.z_offset;
	const tile_t *tiles = tiles_object->tiles;

	for (int i = 0; i < num_tiles; i++) {
		tile_t *tile = &tiles_object->tiles[i];

		// Set indicies

		const int right = i + 1;
		const int left = i - 1;
		const int above = i + num_tiles_per_layer;
		const int above_right_up = i + num_tiles_per_layer - num_cols +
			(tile->is_staggered ? 1 : 0);
		const int right_up = i - num_cols +
			(tile->is_staggered ? 1 : 0);
		const int left_up = i - num_cols -
			(tile->is_staggered ? 0 : 1);
		const int right_down = i + num_cols +
			(tile->is_staggered ? 1 : 0);
		const int left_down = i + num_cols -
			(tile->is_staggered ? 0 : 1);

		// Set blocked flags

		if (above < num_tiles &&
			above >= 0 &&
			tiles[above].is_visible &&
			tiles[above].dstrect.x == tile->dstrect.x
		) {
			tile->is_blocked_from_above = true;
		} else {
			tile->is_blocked_from_above = false;
		}

		if (left_down < num_tiles &&
			left_down >= 0 &&
			tiles[left_down].is_visible &&
			tiles[left_down].dstrect.y == tile->dstrect.y + y_offset
		) {
			tile->is_blocked_from_left_down = true;
		} else {
			tile->is_blocked_from_left_down = false;
		}

		if (right_down < num_tiles &&
			right_down >= 0 &&
			tiles[right_down].is_visible &&
			tiles[right_down].dstrect.y == tile->dstrect.y + y_offset
		) {
			tile->is_blocked_from_right_down = true;
		} else {
			tile->is_blocked_from_right_down = false;
		}

		if (above_right_up < num_tiles &&
			above_right_up >= 0 &&
			tiles[above_right_up].is_visible &&
			tiles[above_right_up].dstrect.y == tile->dstrect.y - y_offset - z_offset
		) {
			tile->is_blocked_from_above_right_up = true;
		} else {
			tile->is_blocked_from_above_right_up = false;
		}

		// Set visibility

		if ((!tile->is_blocked_from_above ||
			!tile->is_blocked_from_left_down ||
			!tile->is_blocked_from_right_down) &&
			tile->is_active
		) {
			tile->is_visible = true;
		} else {
			tile->is_visible = false;
		}

		// // Highlighting
		//
		if (!tile->is_blocked_from_above &&
			mouse.x >= tile->hitbox.x &&
			mouse.x <= tile->hitbox.x + hitbox_size &&
			mouse.y >= tile->hitbox.y &&
			mouse.y <= tile->hitbox.y + hitbox_size
		) {
			tile->is_highlighted = true;
		} else {
			tile->is_highlighted = false;
		}
	}

	return 0;
}

int tiles_draw(const tiles_t *tiles, SDL_Renderer *ren) {
	if (!tiles || !tiles->tex || !ren) {
		SET_ERR("Arguments cannot be NULL.");
		return 1;
	}
	const int num_tiles = tiles->num_tiles;

	static unsigned char old_alpha = 255;
	static unsigned char new_alpha = 255;
	int num_tiles_rendered = 0;
	for (int i = 0; i < num_tiles; i++) {
		const tile_t *tile = &tiles->tiles[i];
		if (!tile->is_visible) continue;

		new_alpha = tile->is_highlighted ? 128 : 255;
		if (new_alpha != old_alpha) {
			if (SDL_SetTextureAlphaMod(tiles->tex, new_alpha)) {
				SET_ERR("Failed to set texture alpha mod.");
				return 1;
			}
		}
		old_alpha = new_alpha;

		if (SDL_RenderCopy(ren, tiles->tex, &tile->srcrect, &tile->dstrect)) {
			SET_ERR("Failed to copy texture.");
			return 1;
		}

		num_tiles_rendered++;
	}
	printf("%d\n", num_tiles_rendered);
	// for (int i = 0; i < num_tiles; i++) {
	// 	const tile_t *tile = &tiles->tiles[i];
	// 	if (SDL_SetRenderDrawColor(ren, 255, 0, 0, 128)) {
	// 		SET_ERR("Failed to set draw color.");
	// 		return 1;
	// 	}
	// 	if (SDL_RenderFillRect(ren, &tile->hitbox)) {
	// 		SET_ERR("Failed to fill rect.");
	// 		return 1;
	// 	}
	// }

	return 0;
}

