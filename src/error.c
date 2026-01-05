#include "sdl.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_error.h>

#define ERR_BUFF_SIZE 512LU

static char g_err[ERR_BUFF_SIZE];

void set_err(const char *msg, const char *file, const char *func, int line) {
	const char *err_header = "[ERROR]: ";
	if (strlen(err_header) +
		strlen(msg) +
		strlen(file) +
		strlen(func) +
		10 +
		strlen(SDL_GetError()) > ERR_BUFF_SIZE
	) {
		msg = "Error buffer overflow.";
	}

	sprintf(g_err, "%s %s\nFile: %s\nFunc: %s\nLine: %d\n%s",
			err_header, msg, file, func, line, SDL_GetError());
}

const char *get_err() {
	return g_err;
}
