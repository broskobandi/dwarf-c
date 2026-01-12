#ifndef ERROR_H
#define ERROR_H

#define SET_ERR(msg)\
	set_err((msg), __FILE__, __func__, __LINE__)

void set_err(const char* msg, const char *file, const char *func, int line);
void print_err();

#endif
