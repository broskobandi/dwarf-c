#!/bin/env sh

make CC=gcc \
	CFLAGS="-Wall -Wextra -Wunused-result -Wconversion -O3 -march=native -flto" \
	CPPFALGS="-Isrc -DNDEBUG" \
	BUILD_DIR=build/linux/release
