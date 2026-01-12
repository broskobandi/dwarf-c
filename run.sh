#!/bin/env sh

. ./build.sh
"./build/$OS/$BUILD_TYPE/game"

STATUS=$?

if [ $STATUS != 0 ]; then
	echo "make run failed."
	exit 1
else
	echo "Game finished with status code: $STATUS"
fi
