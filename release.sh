#!/bin/env sh

make BUILD_TYPE=release \
	CC=gcc \
	CFLAGS="-Wall -Wextra -Wunused-result -Wconversion -O3 -march=native -flto" \
	CPPFLAGS="-Isrc -DNDEBUG"
