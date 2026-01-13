/*
MIT License

Copyright (c) 2025 broskobandi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/** \file game.h
 * \brief Public header file for the Game library.
 * \details Contains public function declarations, macro helpers and the
 * initialization objet. */

#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include <stdint.h>

// Helper macros

#define TRY(func)\
	do {\
		if ((func)) {\
			game_quit();\
			game_print_error();\
			return 1;\
		}\
	} while (0)

#define INIT(init_data)\
	do {\
		TRY(game_init((init_data)));\
	} while(0)

#define RUN()\
	do {\
		TRY(game_run());\
	} while(0)

#define QUIT()\
	do {\
		game_quit();\
	} while(0)


typedef struct game_init_data {
	const char *title;
	int win_w;
	int win_h;
	int has_vsync;
	uint8_t background_r;
	uint8_t background_g;
	uint8_t background_b;
	struct {
		size_t num_layers;
		size_t num_rows;
		size_t num_cols;
	} ground;
} game_init_data_t;

/** Prints the latest error message if exists. */
void game_print_error();

/** Initializes the game instance.
 * \returns 0 on success or 1 on failure.
 * The error message can be queried with 
 * game_print_error(). */
int game_init(game_init_data_t init_data);

/** Runs the game instance.
 * \returns 0 on success or 1 on failure.
 * The error message can be queried with 
 * game_print_error(). */
int game_run();

/** Terminates the game instance. */
void game_quit();

#endif
