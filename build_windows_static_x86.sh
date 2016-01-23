#!/bin/sh

i686-w64-mingw32-g++ -static -static-libgcc -static-libstdc++ -lpthread -Wall -O3 -std=c++14 -o main_x86.exe src/*.cpp
