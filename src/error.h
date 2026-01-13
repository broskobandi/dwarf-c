#ifndef ERROR_H
#define ERROR_H

#define SET_ERR(msg)\
	set_err((msg), __FILE__, __func__, __LINE__)

/** Populates the thread-local global error buffer. */
void set_err(const char *msg, const char *file, const char *func, int line);

/** Prints the current content of the thread-local global error
 * buffer. */
void print_err();

#endif
