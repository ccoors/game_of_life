# Game of Life

## Introduction
This is a simple Game of Life implementation in C++14.

## Features
* Square grid
* Standard 23/3 rule
* Multithreaded (up to 32 threads)
* Supports loading RLE files

### Planned features
* Non-square grid
* Other rules
* Support for other file formats
* Saving

## Requirements
### Build
* CMake (3.1 or later)
* C++14 compiler (g++ 5.3.0, clang++ 3.7.1 tested)

## Build instructions
### Generic
    mkdir build
    cd build
    cmake ..
    make

### Arch Linux
    cd arch-pkgbuild
    makepkg
    sudo pacman -U game_of_life-git-X
Replace X with the actual version number, architecture and file extension.

### Linux crosscompile for Windows
    cd tools
    ./build_windows_static_x{86,64}.sh
