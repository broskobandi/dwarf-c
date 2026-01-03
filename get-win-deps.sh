#!/bin/env sh

mkdir -p build/win/deps

git -C build/win/deps clone https://github.com/broskobandi/vec.git
cd build/win/deps/vec &&
./win-release.sh

cd -

cd build/win/deps

wget https://github.com/libsdl-org/SDL/releases/download/release-2.32.8/SDL2-devel-2.32.8-mingw.zip
unzip SDL2-devel-2.32.8-mingw.zip
rm SDL2-devel-2.32.8-mingw.zip

cd - 
