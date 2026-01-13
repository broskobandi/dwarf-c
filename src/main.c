#include "game.h"

#define WIN_W 800
#define WIN_H 600

int main(void) {

	game_init_data_t init_data = {
		.title = "Dwarf",
		.win_w = WIN_W,
		.win_h = WIN_H,
		.has_vsync = 1,
		.background_r = 30,
		.background_g = 75,
		.background_b = 75,
		.ground = {
			2,
			4,
			6
		}
	};

	INIT(init_data);
	RUN();
	QUIT();
	
	return 0;
}
