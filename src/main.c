#define SDL_MAIN_HANDLED
#include "game.h"
#include "error.h"
#include <stdio.h>

int main(void) {
	if (game_run()) {
		fprintf(stderr, "%s\n", get_err());
		return 1;
	}
	return 0;
}
