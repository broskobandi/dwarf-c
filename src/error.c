#include "error.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_error.h>

#define ERR_BUFF_SIZE 512lu

_Thread_local char g_err[ERR_BUFF_SIZE];

void set_err(const char* msg, const char *file, const char *func, int line) {
	const char *err_header = "[ERR]: ";
	if (	strlen(err_header) +
		strlen(msg) +
		strlen(file) +
		strlen(func) +
		strlen(SDL_GetError()) +
		10 > ERR_BUFF_SIZE
	) {
		msg = "ERR_BUFF_SIZE overflow.";
	}

	sprintf(g_err, "%s%s\nFile: %s\nFunc: %s\nLine: %d\n%s",
		err_header, msg, file, func, line, SDL_GetError()
	);
}

void print_err() {
	fprintf(stderr, "%s\n", g_err);
}
