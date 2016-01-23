#ifndef IMPORT_H
#define IMPORT_H

#include "main.h"

#include "game_of_life.h"

namespace import {

bool import_rle(std::ifstream &i, gol::Game_of_life &g, const int x,
                const int y);
}

#endif
