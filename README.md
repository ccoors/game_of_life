# Game of Life

## Introduction
This is a simple Game of Life implementation in C++14.

## Features
* 2D grid
* Any possible rule can be selected (Stay Alive/Born)
* Needs only C++ Standard library
* Multithreaded (up to 32 threads)
  * Only useful for large grids (> 1000 x 1000)
* Supported loading file formats:
  * Run Length Encoded (.rle)
  * Plaintext (.cells)
  * Life 1.06 (.lif or .life) files
* Supported saving file formats:
  * Plaintext (.cells)

## Requirements
### Build
* CMake (3.1 or later)
* C++14 compiler (g++ 5.3.0, clang++ 3.7.1 tested)

## Build instructions
### Generic
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make

### Arch Linux
    cd arch-pkgbuild
    makepkg
    sudo pacman -U game_of_life-git-X
Replace X with the actual version number, architecture and file extension.

### Linux crosscompile for Windows
    cd tools
    ./build_windows_static_x{86,64}.sh

## Development
This project relies on several optimizations done by modern compilers and
linkers like return value optimization (RVO) and improvements in C++ like
move semantics. Therefore, things like return-by-value are used in most
cases to improve readability.
