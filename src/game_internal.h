#ifndef GAME_INTERNAL
#define GAME_INTERNAL

#define SDL_MAIN_HANDLED
#include "game.h"
#include "ground.h"
#include "entity.h"
#include "error.h"
#include "debug.h"
#include "stddef.h"
#include <SDL2/SDL.h>
#include <string.h>

#define TEXTURES_BUFF_SIZE 64

struct game {
	SDL_Texture *textures[TEXTURES_BUFF_SIZE];
	const char *loaded_texture_paths[TEXTURES_BUFF_SIZE];
	size_t num_tex;
	SDL_Window *win;
	SDL_Renderer *ren;
	ground_t *ground;
	entity_t *entity;
};

_Thread_local static struct game g_game;
_Thread_local static int g_is_init;

static inline size_t load_texture(const char *path_to_bmp) {
	if (!g_is_init) {
		SET_ERR("Game must be initialized before using load_texture().");
		return (size_t)-1;
	}

	if (!path_to_bmp) {
		SET_ERR("Invalid argument.");
		return (size_t)-1;
	}

	for (size_t i = 0; i < g_game.num_tex; i++) {
		if (!strcmp(g_game.loaded_texture_paths[i], path_to_bmp)) {
			DBG("Texture has already been loaded.");
			return i;
		}
	}

	if (g_game.num_tex + 1 > TEXTURES_BUFF_SIZE) {
		SET_ERR("TEXTURES_BUFF_SIZE overflow.");
		return (size_t)-1;
	}

	SDL_Surface *sur = SDL_LoadBMP(path_to_bmp);
	if (!sur) {
		SET_ERR("Failed to create surface.");
		return (size_t)-1;
	}
	DBG("Surface created.");

	g_game.textures[g_game.num_tex] =
		SDL_CreateTextureFromSurface(g_game.ren, sur);
	if (!g_game.textures[g_game.num_tex]) {
		SET_ERR("Failed to create texture.");
		SDL_FreeSurface(sur);
		DBG("Surface freed.");
		return (size_t)-1;
	}
	DBG("Texture created.");

	g_game.num_tex++;

	SDL_FreeSurface(sur);
	DBG("Surface freed.");

	return g_game.num_tex - 1;
}

static inline int render_ground() {
	if (!g_is_init) {
		SET_ERR("Game must be initialized before running render_ground()");
		return 1;
	}

	const blocks_vec_t *blocks = ground_get_blocks_vec(g_game.ground);

	for (size_t layer = 0; layer < blocks->num_layers; layer++) {
	 for (size_t row = 0; row < blocks->num_rows; row++) {
	  for (size_t col = 0; col < blocks->num_cols; col++) {
	   block_t *block = &blocks->blocks[layer][row][col];
	   SDL_Texture *tex = g_game.textures[block->tex_id];
	   SDL_RenderCopyF(
		g_game.ren,
		tex,
		&block->srcrect,
		&block->dstrect
	   );
	  }
	 }
	}

	return 0;
}

#endif
