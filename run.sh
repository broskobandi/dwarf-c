#!/bin/env sh

. ./build.sh
if [ "$OS" = "windows" ]; then
	wine "./build/$OS/$BUILD_TYPE/game.exe"
else
	"./build/$OS/$BUILD_TYPE/game"
fi

echo "Running $BUILD_TYPE build for $OS exited with status code: $?."
