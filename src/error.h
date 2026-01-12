#ifndef ERROR_H
#define ERROR_H

#define SET_ERR(msg)\
	set_err((msg), __FILE__, __func__, __LINE__)

/** Sets the internal error state.
 * \param msg The error message.
 * \param file The name of the current file.
 * \param func The name of the current function. 
 * \param line The current line. */
void set_err(const char* msg, const char *file, const char *func, int line);

/** Prints the current error state. */
void print_err();

#endif
