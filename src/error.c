#include "error.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <SDL2/SDL_error.h>

#define ERR_BUFF_SIZE 512lu

_Thread_local char g_err[ERR_BUFF_SIZE];

void set_err(const char *msg, const char *file, const char *func, int line) {

	const char *header = "[ERR]: ";

	size_t err_length =
		strlen(msg) +
		strlen(file) +
		strlen(func) +
		10 +
		strlen(SDL_GetError());

	if (err_length >= ERR_BUFF_SIZE) {
		msg = "ERR_BUFF_SIZE overflow.";
	}

	sprintf(g_err, "%s%s\n\tFile: %s\n\tFunc: %s\n\tLine: %d\n\t%s",
		header, msg, file, func, line, SDL_GetError()
	);

}

const char *get_err() {
	return g_err;
}
