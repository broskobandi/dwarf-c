#include <stdio.h>
#define SDL_MAIN_HANDLED
#include "game.h"
#include "error.h"

int main(void) {

	if (game_run()) {
		printf("%s\n", get_err());
		return 1;
	}

	return 0;
}
