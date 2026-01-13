#include "error.h"
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL_error.h>

#define ERR_BUFF_SIZE 512lu

_Thread_local char g_err[ERR_BUFF_SIZE];

/** Populates the thread-local global error buffer. */
void set_err(const char *msg, const char *file, const char *func, int line) {
	const char *header = "[ERR]: ";

	if (	strlen(msg) +
		strlen(header) +
		strlen(file) +
		strlen(func) +
		10 + 
		strlen(SDL_GetError()) > ERR_BUFF_SIZE
	) {
		msg = "ERR_BUFF_SIZE overflow.";
	}

	sprintf(g_err, "%s\t%s\n\tFile: %s\n\tFunc: %s\n\tLine: %d\n\t%s",
		header, msg, file, func, line, SDL_GetError()
	);
}

/** Prints the current content of the thread-local global error
 * buffer. */
void print_err() {
	fprintf(stderr, "%s\n", g_err);
}
