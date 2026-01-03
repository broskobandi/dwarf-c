#!/bin/env sh

if [ ! -d build/win/deps ]; then
	./get-win-deps.sh
fi

VEC_LIB_DIR=build/win/deps/vec/build/win/release/lib
VEC_INC_DIR=build/win/deps/vec/include
SDL_LIB_DIR=build/win/deps/SDL2-2.32.8/x86_64-w64-mingw32/lib
SDL_BIN_DIR=build/win/deps/SDL2-2.32.8/x86_64-w64-mingw32/bin
SDL_INC_DIR=build/win/deps/SDL2-2.32.8/x86_64-w64-mingw32/include

make CC=x86_64-w64-mingw32-gcc \
	CPPFALGS="-Isrc -I$VEC_INC_DIR -I$SDL_INC_DIR" \
	LDFLAGS="-L$VEC_LIB_DIR -lvec -L$SDL_LIB_DIR -lSDL2" \
	BUILD_DIR=build/win/debug \
	BIN="\$(BUILD_DIR)/\$(PROJECT).exe"

cp $SDL_BIN_DIR/SDL2.dll $VEC_LIB_DIR/libvec.dll build/win/debug/
