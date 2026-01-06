#include "ground.h"
#include "error.h"
#include <stdbool.h>
#include <stddef.h>

int ground_init(ground_t *g) {
	if (!g) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	g->num_tiles_per_layer = g->num_cols * g->num_rows;

	for (int z = 0; z < g->num_layers; z++) {
		for (int y = 0; y < g->num_rows; y++) {
			float x_offset = y % 2 == 0 ? 0 : g->dstrect_size / 2;
			for (int x = 0; x < g->num_cols; x++) {
				if (g->num_tiles + 1 > MAX_NUM_TILES) {
					SET_ERR("Tiles buffer overflow.");
					return 1;
				}

				tile_t tile = {
					.dstrect = {
						.x = (float)x * g->dstrect_size + x_offset,
						.y = (float)y - g->spatial_offset - (g->z_offset * (float)z),
						.w = g->dstrect_size,
						.h = g->dstrect_size
					},
					.srcrect = {
						.x = 0,
						.y = 0,
						.w = g->srcrect_size,
						.h = g->srcrect_size
					},
					.hitbox = {
						.x = tile.dstrect.x + (g->dstrect_size - g->hitbox_size) / 2,
						.y = tile.dstrect.y,
						.w = g->hitbox_size,
						.h = g->dstrect_size
					},
					.top = {
						.x = tile.dstrect.x + g->dstrect_size / 2,
						.y = tile.dstrect.y,
						.z = tile.dstrect.y + g->spatial_offset
					},
					.is_highlighted = false,
					.is_selected = false,
					.is_blocked_from_above = false
				};
				g->tiles[g->num_tiles] = tile;
				g->num_tiles++;
			}
		}
	}

	return 0;
}

int ground_update(ground_t *g, SDL_Point mouse, bool left_click) {
	if (!g) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	for (size_t i = 0; i < g->num_tiles; i++) {
		tile_t *tile = &g->tiles[i];
		if ((float)mouse.x >= tile->dstrect.x &&
			(float)mouse.x <= tile->dstrect.x + g->dstrect_size &&
			(float)mouse.y >= tile->dstrect.y &&
			(float)mouse.x <= tile->dstrect.y &&
			!tile->is_blocked_from_above
		) {
			tile->is_highlighted = true;
		} else {
			tile->is_highlighted = false;
		}
		if (tile->is_highlighted && left_click) {
			if (!tile->is_selected) {
				tile->is_selected = true;
			} else {
				tile->is_selected = false;
			}
		}
	}
	
	return 0;
}

void ground_del(ground_t *g) {
	if (g) {
		if (g->tex) {
			SDL_DestroyTexture(g->tex);
			DBG("Ground texture destroyed.");
		}
	}
}
