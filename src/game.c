#define SDL_MAIN_HANDLED
#include "game.h"
#include "error.h"
#include "debug.h"
#include "ground.h"
#include "blocks.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <SDL2/SDL.h>

#define CAP_TEX_DEFAULT 16

typedef struct game {
	game_init_data_t init_data;
	SDL_Texture **textures;
	const char **loaded_tex_paths;
	size_t num_tex;
	size_t cap_tex;
	SDL_Window *win;
	SDL_Renderer *ren;
	int is_sdl_init;
	ground_t *ground;
} game_t;

_Thread_local static game_t g_game;

static inline int set_draw_color(SDL_Color col);
static inline size_t create_texture(const char *path);
static inline int render_texture(
	SDL_Texture *tex,
	const SDL_FRect *dstrect,
	const SDL_Rect *srcrect,
	size_t img_index
);
static inline int render_content();

int game_init(
	game_init_data_t game_init_data,
	blocks_init_data_t blocks_init_data
) {
	if (g_game.is_sdl_init) {
		SET_ERR("Cannot initialize game twice.");
		return 1;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		SET_ERR("Failed to initialize SDL.");
		return 1;
	}
	g_game.is_sdl_init = 1;
	DBG("SDL initialized.");

	g_game.win = SDL_CreateWindow(
		game_init_data.title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		game_init_data.win_w,
		game_init_data.win_h,
		SDL_WINDOW_SHOWN
	);
	if (!g_game.win) {
		SET_ERR("Failed to create window.");
		game_quit();
		return 1;
	}
	DBG("Window created.");

	g_game.ren = SDL_CreateRenderer(
		g_game.win,
		-1,
		game_init_data.has_vsync ? SDL_RENDERER_PRESENTVSYNC : 0
	);
	if (!g_game.ren) {
		SET_ERR("Failed to create renderer.");
		game_quit();
		return 1;
	}
	DBG("Renderer created.");

	if (SDL_SetRenderDrawBlendMode(g_game.ren, SDL_BLENDMODE_BLEND)) {
		SET_ERR("Failed to set render draw blend mode.");
		game_quit();
		return 1;
	}

	g_game.textures = calloc(CAP_TEX_DEFAULT, sizeof(SDL_Texture*));
	if (!g_game.textures) {
		SET_ERR("Failed to allocate textures buffer.");
		game_quit();
		return 1;
	}
	DBG("Textures buffer allocated.");

	g_game.loaded_tex_paths = calloc(CAP_TEX_DEFAULT, sizeof(const char*));
	if (!g_game.loaded_tex_paths) {
		SET_ERR("Failed to allocate loaded_tex_paths buffer.");
		game_quit();
		return 1;
	}
	DBG("loaded_tex_paths buffer allocated.");

	g_game.num_tex = 0;
	g_game.cap_tex = CAP_TEX_DEFAULT;

	g_game.init_data = game_init_data;

	size_t ground_tex_id = create_texture(blocks_init_data.path_to_bmp);
	if (ground_tex_id == -1lu) {
		game_quit();
		return 1;
	}
	g_game.ground = ground_new(blocks_init_data, ground_tex_id);
	if (!g_game.ground) {
		game_quit();
		return 1;
	}

	return 0;
}

int game_run() {
	if (!g_game.is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	SDL_Event event;
	int is_running = 1;
	const SDL_Color bg = {
		.r = g_game.init_data.bg_r,
		.g = g_game.init_data.bg_g,
		.b = g_game.init_data.bg_b,
		255
	};

	DBG("Running main loop...");

	while (is_running) {
		int is_left_click = 0;
		int is_right_click = 0;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT ||
			   (event.type == SDL_KEYDOWN &&
			    event.key.keysym.sym == SDLK_q)
			) {
				is_running = 0;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					is_left_click = 1;
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					is_right_click = 1;
				}
			}
		}

		if (set_draw_color(bg)) {
			return 1;
		}

		if (SDL_RenderClear(g_game.ren)) {
			SET_ERR("Failed to clear renderer.");
			return 1;
		}

		SDL_Point mouse = {0};
		SDL_GetMouseState(&mouse.x, &mouse.y);

		if (ground_update(
			g_game.ground,
			mouse,
			is_left_click,
			is_right_click)
		) {
			return 1;
		}

		if (render_content()) {
			return 1;
		}

		SDL_RenderPresent(g_game.ren);

	}

	DBG("Main loop finished.");

	return 0;
}

static inline int set_draw_color(SDL_Color col) {
	if (!g_game.is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	if (SDL_SetRenderDrawColor(g_game.ren, col.r, col.g, col.b, col.a)) {
		SET_ERR("Failed to set render draw color.");
		return 1;
	}

	return 0;
}

static inline size_t create_texture(const char *path) {
	if (!g_game.is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return (size_t)-1;
	}

	if (!path) {
		SET_ERR("Invalid argument.");
	}


	for (size_t i = 0; i < g_game.num_tex; i++) {
		if (!strcmp(g_game.loaded_tex_paths[i], path)) {
			DBG("Texture has already been loaded.");
			return i;
		}
	}


	if (g_game.num_tex + 1 > g_game.cap_tex) {
		size_t new_cap =
			(size_t)((float)g_game.cap_tex * 1.5f);

		SDL_Texture **tmp = realloc(g_game.textures, new_cap);
		if (!tmp) {
			SET_ERR("Failed to expand textures buffer.");
			return (size_t)-1;
		}
		DBG("Textures buffer expanded.");

		const char **tmp2 = realloc(g_game.loaded_tex_paths, new_cap);
		if (!tmp2) {
			SET_ERR("Failed to expand loaded_tex_paths buffer.");
			return (size_t)-1;
		}
		DBG("loaded_tex_paths expanded.");

		g_game.textures = tmp;
		g_game.loaded_tex_paths = tmp2;

		g_game.cap_tex = new_cap;
	}


	SDL_Surface *sur = SDL_LoadBMP(path);
	if (!sur) {
		SET_ERR("Failed to create surface.");
		return (size_t)-1;
	}
	DBG("Surface created.");


	SDL_Texture *tex = SDL_CreateTextureFromSurface(g_game.ren, sur);
	if (!tex) {
		SET_ERR("Failed to create texture.");
		SDL_FreeSurface(sur);
		DBG("Surface freed.");
		return (size_t)-1;
	}
	DBG("Texture created.");

	SDL_FreeSurface(sur);
	DBG("Surface freed.");

	g_game.textures[g_game.num_tex] = tex;
	g_game.loaded_tex_paths[g_game.num_tex] = path;

	g_game.num_tex++;

	return g_game.num_tex - 1;
}

static inline int render_texture(
	SDL_Texture *tex,
	const SDL_FRect *dstrect,
	const SDL_Rect *srcrect,
	size_t img_index
) {
	if (!g_game.is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	if (!tex) {
		SET_ERR("Invalid argument.");
		return 1;
	}

	SDL_Rect src = *srcrect;
	src.x = src.w * (int)img_index;

	if (!SDL_RenderCopyF(
		g_game.ren,
		tex,
		&src,
		dstrect)
	) {
		SET_ERR("Failed to render texture.");
		return 1;
	}

	return 0;
}

static inline int render_content() {
	if (!g_game.is_sdl_init) {
		SET_ERR("Game must be initialized first.");
		return 1;
	}

	const blocks_t *blocks = ground_get_blocks(g_game.ground);

	if (blocks->tex_id >= g_game.num_tex) {
		SET_ERR("blocks tex_id is out of bounds.");
		return 1;
	}
	SDL_Texture *tex = g_game.textures[blocks->tex_id];

	const size_t num_layers = blocks->init_data.num_layers;
	const size_t num_rows = blocks->init_data.num_rows;
	const size_t num_cols = blocks->init_data.num_cols;
	const size_t right_shadow_index = blocks->init_data.right_shadow_index;
	const size_t left_shadow_index = blocks->init_data.left_shadow_index;
	const size_t right_light_index = blocks->init_data.right_light_index;
	const size_t top_shadow_index = blocks->init_data.top_shadow_index;
	const size_t highlighted_index = blocks->init_data.highlighted_index;
	const size_t selected_index = blocks->init_data.selected_index;

	for (size_t layer = 0; layer < num_layers; layer++) {
		for (size_t row = 0; row < num_rows; row++) {
			for (size_t col = 0; col < num_cols; col++) {
				const block_t *block =
					&blocks->blocks[layer][row][col];
				if (!block->is_active) continue;

				if (!render_texture(
					tex,
					&block->dstrect,
					&block->srcrect,
					0)
				) {
					return 1;
				}

				if (block->has_right_shadow) {
					if (!render_texture(
						tex,
						&block->dstrect,
						&block->srcrect,
						right_shadow_index)
					) {
						return 1;
					}
				}

				if (block->has_left_shadow) {
					if (!render_texture(
						tex,
						&block->dstrect,
						&block->srcrect,
						left_shadow_index)
					) {
						return 1;
					}
				}

				if (block->has_right_light) {
					if (!render_texture(
						tex,
						&block->dstrect,
						&block->srcrect,
						right_light_index)
					) {
						return 1;
					}
				}

				if (block->has_top_shadow) {
					if (!render_texture(
						tex,
						&block->dstrect,
						&block->srcrect,
						top_shadow_index)
					) {
						return 1;
					}
				}

				if (block->is_highlighted) {
					if (!render_texture(
						tex,
						&block->dstrect,
						&block->srcrect,
						highlighted_index)
					) {
						return 1;
					}
				}
			}
		}
	}

	const block_t *selected_block = 
		ground_get_selected_block(
			g_game.ground
		);

	if (selected_block) {
		if (!render_texture(
			tex,
			&selected_block->dstrect,
			&selected_block->srcrect,
			selected_index)
		) {
			return 1;
		}
	}

	return 0;
}

const char *game_get_error() {
	return get_err();
}

void game_quit() {
	if (g_game.ground) {
		ground_del(g_game.ground);
	}
	if (g_game.loaded_tex_paths) {
		free(g_game.loaded_tex_paths);
		DBG("loaded_tex_paths buffer freed.");
	}
	if (g_game.textures) {
		for (size_t i = 0; i < g_game.num_tex; i++) {
			if (g_game.textures[i]) {
				SDL_DestroyTexture(g_game.textures[i]);
				DBG("Texture destroyed.");
			}
		}
		free(g_game.textures);
		DBG("Textures buffer freed.");
	}
	if (g_game.ren) {
		SDL_DestroyRenderer(g_game.ren);
		DBG("Renderer destroyed.");
	}
	if (g_game.win) {
		SDL_DestroyWindow(g_game.win);
		DBG("Window destroyed.");
	}
	if (g_game.is_sdl_init) {
		SDL_Quit();
		DBG("SDL terminated.");
	}
}
