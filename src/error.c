#include "error.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL_error.h>

/** Buffer for storing the error information in. */
#define ERR_BUFF_SIZE 512lu

/** Thread-local global error char buffer. */
_Thread_local char g_err[ERR_BUFF_SIZE];

/** Sets the internal error state.
 * \param msg The error message.
 * \param file The name of the current file.
 * \param func The name of the current function. 
 * \param line The current line. */
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

/** Prints the current error state. */
void print_err() {
	fprintf(stderr, "%s\n", g_err);
}
