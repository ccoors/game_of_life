#ifndef UI_H
#define UI_H

#include "main.h"

#include "game_of_life.h"
#include "commands.h"

namespace ui {

void print_line(const int count, const std::string s);
std::string repeat(const int count, const std::string s);
bool command_prompt(gol::Game_of_life& g);
bool input_int(int& i, bool unsign = false);
}

#endif
