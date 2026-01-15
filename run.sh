#!/bin/env sh

CC="clang"
CFLAGS="-Wall -Wextra -Wunused-result -Wconversion"
CPPFLAGS="-Isrc"
LDFLAGS="-lSDL2"
OS="linux"
BUILD_TYPE="debug"

SDL_DIR="SDL2-2.32.8/x86_64-w64-mingw32"

for arg in "$@"; do
	if	[ "$arg" != "-w" ]		&&
		[ "$arg" != "--windows" ]	&&
		[ "$arg" != "-l" ]		&&
		[ "$arg" != "--linux" ]		&&
		[ "$arg" != "-d" ]		&&
		[ "$arg" != "--debug" ]		&&
		[ "$arg" != "-r" ]		&&
		[ "$arg" != "--release" ]	; then

		echo "
Usage: $0 [OPTIONS]

Options:

	-w | --windows	= Compile for Windows.
	-l | --linux	= Compile for Linux (default).
	-d | --debug 	= Compile debug build (default).
	-r | --release	= Compile release build.
	-h | --help	= Print this message.
"
	fi

	if	[ "$arg" = "-w" ]		||
		[ "$arg" = "--windows" ]	;then

		CC="x86_64-w64-mingw32-gcc"
		CPPFLAGS="$CPPFLAGS -I $SDL_DIR/include"
		LDFLAGS="-L $SDL_DIR/lib -l SDL2"
		OS="windows"
	fi

	if 	[ "$arg" = "-r" ] 		||
		[ "$arg" = "--release" ] 	;then

		if [ "$OS" = "linux" ]; then
			CC="gcc"
		fi
		CFLAGS="$CFLAGS -O3 -march=native -flto"
		CPPFLAGS="$CPPFLAGS -DNDEBUG"
		BUILD_TYPE="release"
	fi
done

if [ "$OS" = "windows" ]; then
	echo "Compiling for Windows."
else
	echo "Compiling for Linux."
fi

if [ "$BUILD_TYPE" = "release" ]; then
	echo "Compiling release build."
else
	echo "Compiling debug build."
fi

make				\
	CC="$CC"		\
	CFLAGS="$CFLAGS"	\
	CPPFLAGS="$CPPFLAGS"	\
	LDFLAGS="$LDFLAGS" 	\
	OS="$OS"		\
	BUILD_TYPE="$BUILD_TYPE"

if [ "$?" != 0 ]; then
	echo "Build failed."
	exit 1
fi

if [ "$OS" = "windows" ];then
	cp "$SDL_DIR/bin/SDL2.dll" "build/$OS/$BUILD_TYPE"
	cp "/usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll" "build/$OS/$BUILD_TYPE"
	wine "build/$OS/$BUILD_TYPE/game.exe" 
else
	"./build/$OS/$BUILD_TYPE/game"
fi

echo "Running $BUILD_TYPE build for $OS finished with status code: $?"
