#!/bin/env sh

. ./build.sh
if [ "$OS" = "windows" ]; then
	wine "./build/$OS/$BUILD_TYPE/game.exe"
else
	"./build/$OS/$BUILD_TYPE/game"
fi

STATUS=$?

if [ $STATUS != 0 ]; then
	echo "Game failed with status code: $STATUS."
	exit 1
else
	echo "Game finished with status code: $STATUS"
fi
