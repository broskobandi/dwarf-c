#!/bin/bash

if [[ "$1" != "debug" && "$1" != "release" ]]; then
	echo "Please specify \"release\" or \"debug\""
	exit
fi

OS=win
BUILD_TYPE="$1"
CC=x86_64-w64-mingw32-gcc
DEPS_DIR=build/deps
BUILD_DIR="build/$OS/$BUILD_TYPE"
SDL_DIR="$DEPS_DIR/SDL2-2.32.8/x86_64-w64-mingw32"
LDFLAGS="-L $SDL_DIR/lib -lSDL2"
# BIN_DIR="$BUILD_DIR/bin"
# OUTPUT_DIR="$BUILD_DIR/output"
if [[ "$1" == "release" ]]; then
	CFLAGS="-Wall -Wextra -Wconversion -Wunused-result -O3 -march=native -flto"
else
	CFLAGS="-Wall -Wextra -Wconversion -Wunused-result"
fi
if [[ "$1" == "release" ]]; then
	CPPFLAGS="-Isrc -I$SDL_DIR/include -DNDEBUG"
else
	CPPFLAGS="-Isrc -I$SDL_DIR/include"
fi

if [[ ! -d $DEPS_DIR ]]; then
	mkdir -p "$DEPS_DIR" &&
	cd "$DEPS_DIR" &&
	wget "https://github.com/libsdl-org/SDL/releases/download/release-2.32.8/SDL2-devel-2.32.8-mingw.zip" &&
	unzip * &&
	rm *.zip &&
	cd -
fi

make OS="$OS" \
	BUILD_TYPE="$BUILD_TYPE" \
	CC="$CC" \
	CFLAGS="$CFLAGS" \
	CPPFLAGS="$CPPFLAGS" \
	LDFLAGS="$LDFLAGS"

# mkdir -p $OUTPUT_DIR/bin
# cp $BIN_DIR/* $OUTPUT_DIR/bin/ &&
# cp $SDL_DIR/bin/SDL2.dll $OUTPUT_DIR/bin/ &&
# cp -r assets $OUTPUT_DIR/ 
cp $SDL_DIR/bin/SDL2.dll $BUILD_DIR/ &&
cp -r assets $BUILD_DIR/ 
