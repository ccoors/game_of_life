#include "main.h"
#include "ui.h"
#include "game_of_life.h"
#include "import_export.h"

int main() {
  std::cout << "Conway's Game of Life\n";

  gol::Game_of_life g{20, 20};
  std::cout << g;

  while (ui::command_prompt(g)) {
  }

  std::cout << std::endl;
  return 0;
}
