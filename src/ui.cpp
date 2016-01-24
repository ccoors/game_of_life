#include "ui.h"

namespace ui {

void print_line(const int count, const std::string s) {
  std::cout << repeat(count, s) << "\n";
}

std::string repeat(const int count, const std::string s) {
  std::string ret{""};
  for (int i = 0; i < count; i++) {
    ret.append(s);
  }
  return ret;
}

bool command_prompt(gol::Game_of_life &g) {
  std::cout << "Command (? for help): ";
  std::string command;
  bool ok = static_cast<bool>((std::cin >> command));
  if (!ok) {
    std::cout << "exit";
  }
  std::cout << "\n";
  if (!ok) {
    return false;
  }

  return commands::command(command, g);
}

bool input_int(int &i, bool unsign, bool allow_zero) {
  if (std::cin >> i) {
    if (allow_zero)
      return !(i < 0);
    else
      return !(unsign && i < 1);
  }
  std::cin.clear();
  std::cin.ignore();
  return false;
}
} // namespace ui
