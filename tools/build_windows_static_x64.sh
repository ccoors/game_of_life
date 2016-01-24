#!/bin/sh

set -e
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
cd $SCRIPTPATH
x86_64-w64-mingw32-g++ -static -static-libgcc -static-libstdc++ -lpthread -Wall -O3 -std=c++14 -o main_x64.exe ../src/*.cpp
