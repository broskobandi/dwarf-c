#include "error.h"
#include <SDL2/SDL_error.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_BUFF_SIZE 512LU

_Thread_local char g_err[ERR_BUFF_SIZE];

void set_err(const char *msg, const char *file, const char *func, int line) {
	size_t err_size =
		strlen(msg) +
		strlen(file) +
		strlen(func) +
		strlen(SDL_GetError()) +
		20;

	if (err_size > ERR_BUFF_SIZE) {
		fprintf(stderr, "[ERROR]: ERR_BUFF_SIZE overflow.");
		exit(1);
	}

	sprintf(g_err, "[ERROR]: %s\nFile: %s\nFunc: %s\nLine: %d\n%s", 
		msg, file, func, line, SDL_GetError()
	);
}

const char *get_err() {
	return g_err;
}
