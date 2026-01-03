#define SDL_MAIN_HANDLED
#include <vec.h>
#include <stddef.h>
#include <SDL2/SDL.h>

VEC_TYPEDEF(SDL_Rect);

// int main(int argc, char *argv[]) {
int main(void) {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* win = SDL_CreateWindow("", 0, 0, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_Event event;

	VEC(SDL_Rect) rects = VEC_NEW(SDL_Rect);
	SDL_Rect rect1 = {0, 0, 100, 100};
	SDL_Rect rect2 = {200, 0, 100, 100};
	VEC_PUSH(rects, rect1);
	VEC_PUSH(rects, rect2);

	int is_running = 1;

	while (is_running) {
		while (SDL_PollEvent(&event)) {}
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q]) is_running = 0;
		SDL_SetRenderDrawColor(ren, 30, 70, 70, 255);
		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 255, 70, 70, 255);

		for (size_t i = 0; i < VEC_SIZE(rects); i++) {
			SDL_RenderFillRect(ren, VEC_AT_CONST(rects, i));
		}

		SDL_RenderPresent(ren);
	}

	return 0;
}
