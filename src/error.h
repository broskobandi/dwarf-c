#ifndef ERROR_H
#define ERROR_H

#include "debug.h"
#define SET_ERR(msg)\
	do {\
		set_err((msg), __FILE__, __func__, __LINE__);\
		DBG("***ERROR***");\
	} while(0)

void set_err(const char *msg, const char *file, const char *func, int line);
const char *get_err();

#endif
