#include "main.h"
#include "ui.h"
#include "game_of_life.h"
#include "import_export.h"

int main() {
  std::cout << "Conway's Game of Life\n";

  try {
    gol::Game_of_life g{20};
    std::cout << g;

    while (ui::command_prompt(g)) {
    }
  } catch (...) {
    std::cerr << "An uncaught exception was thrown.\n";
  }

  std::cout << std::endl;
  return 0;
}
