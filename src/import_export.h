#ifndef IMPORT_EXPORT_H
#define IMPORT_EXPORT_H

#include "main.h"

#include "game_of_life.h"
#include "util.h"

namespace import_export {

bool import_rle(std::istream &i, gol::Game_of_life &g, const int x,
                const int y);
bool import_cells(std::istream &i, gol::Game_of_life &g, const int x,
                  const int y);
bool import_life(std::istream &i, gol::Game_of_life &g, const int x,
                 const int y);
bool export_cells(const std::string &filename, const gol::Game_of_life &g);
}

#endif
