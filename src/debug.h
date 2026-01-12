#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifndef NDEBUG
#define DBG(msg)\
	printf("[DBG]: %s\n", (msg))
#else
#define DBG(msg)
#endif

#endif
