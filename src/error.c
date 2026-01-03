#include "error.h"
#include <SDL2/SDL_error.h>
#include <string.h>
#include <stdio.h>

#define ERR_BUFF_SIZE 512LU

static char g_err[ERR_BUFF_SIZE];

void set_err(const char *msg, const char *file, const char *func, int line) {
	const char *err_header = "[ERROR]: ";
	if (strlen(err_header) +
		strlen(file) +
		strlen(func) +
		strlen(SDL_GetError()) +
		10 > ERR_BUFF_SIZE
	) {
		msg = "Error buffer overflow.";
	}

	sprintf(g_err, "%s %s\nFile: %s\nFunc %s\n Line: %d\nSDL_ERROR: %s\n",
		err_header, msg, file, func, line, SDL_GetError()
	);
}

const char *get_err() {
	return g_err;
}
