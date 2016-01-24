#ifndef IMPORT_H
#define IMPORT_H

#include "main.h"

#include "game_of_life.h"

namespace import {

bool import_rle(std::istream &i, gol::Game_of_life &g, const int x,
                const int y);
bool import_cells(std::istream &i, gol::Game_of_life &g, const int x,
                  const int y);
bool import_life(std::istream &i, gol::Game_of_life &g, const int x,
                 const int y);
}

#endif
