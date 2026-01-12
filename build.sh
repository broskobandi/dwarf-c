#!/bin/env sh

HELP="
Usage: $0 [OPTIONS]

Options:

-w | --windows	= Compile for windows.
-l | --linux	= Compile for linux (default).
-d | --debug	= Compile debug build (default).
-r | --release 	= Compile release build.
-h | --help 	= Print this message.
"

OS=linux
BUILD_TYPE=debug
CC=clang
CFLAGS="-Wall -Wextra -Wunused-result -Wconversion"
CPPFLAGS="-Isrc"
LDFLAGS="-lSDL2"
SDL_DIR="SDL2-2.32.8/x86_64-w64-mingw32"

for arg in $@; do
	if	[ "$arg" != "-w" ] && [ "$arg" != "--windows" ] &&
		[ "$arg" != "-l" ] && [ "$arg" != "--linux" ] &&
		[ "$arg" != "-d" ] && [ "$arg" != "--debug" ] &&
		[ "$arg" != "-r" ] && [ "$arg" != "--release" ];
	then
		echo "$HELP"
		exit 1
	fi

	if 	[ "$arg" = "-w" ] || [ "$arg" = "--windows" ]; then
		OS=windows
		CC=x86_64-w64-mingw32-gcc
		CPPFLAGS="$CPPFLAGS -I$SDL_DIR/include"
		LDFLAGS="-L$SDL_DIR/lib -lSDL2"
	fi

	if 	[ "$arg" = "-r" ] || [ "$arg" = "--release" ]; then
		BUILD_TYPE=release
		CFLAGS="$CFLAGS -O3 -march=native -flto"
	fi
done

echo "Building for $OS"
echo "Build type: $BUILD_TYPE"

make					\
	OS="$OS"			\
	BUILD_TYPE="$BUILD_TYPE" 	\
	CC="$CC"		 	\
	CFLAGS="$CFLAGS"		\
	CPPFLAGS="$CPPFLAGS"		\
	LDFLAGS="$LDFLAGS"		\

if [ $? != 0 ]; then
	echo "Build failed."
	exit 1
fi

echo "Build finished."

if [ "$OS" = "windows" ]; then
	cp "$SDL_DIR/bin/SDL2.dll" "build/$OS/$BUILD_TYPE/"
	cp "/usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll" "build/$OS/$BUILD_TYPE/"
fi
