#ifndef COMMANDS_H
#define COMMANDS_H

#include "main.h"
#include "game_of_life.h"
#include "ui.h"

namespace commands {

struct command_structure {
  std::string name;
  std::string help;
  std::string shortcut;
  bool (*function)(gol::Game_of_life& g);
  std::string description;
};

bool help(gol::Game_of_life& g);
bool print_grid(gol::Game_of_life& g);
bool clear_grid(gol::Game_of_life& g);
bool resize_grid(gol::Game_of_life& g);
bool set_cell(gol::Game_of_life& g);
bool unset_cell(gol::Game_of_life& g);
bool fill_grid(gol::Game_of_life& g);
bool set_threads(gol::Game_of_life& g);
bool create_glider(gol::Game_of_life& g);
bool run(gol::Game_of_life& g);
bool step(gol::Game_of_life& g);
bool pexit(gol::Game_of_life& g);

bool command(std::string command, gol::Game_of_life& g);
}

#endif
