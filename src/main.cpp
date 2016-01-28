#include "main.h"
#include "ui.h"
#include "game_of_life.h"
#include "import_export.h"

int main() {
  std::cout << "CONVAYS GAME OF LIFE\n\n";

  gol::Game_of_life g{20};
  std::cout << g;

  while (ui::command_prompt(g)) {
  }

  std::cout << std::endl;
  return 0;
}
