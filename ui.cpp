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
  bool ok = (std::cin >> command) ? true : false;
  std::cout << "\n";
  if (!ok) return false;

  return commands::command(command, g);
}

bool input_int(int &i, bool unsign) {
  if (std::cin >> i) {
    if (unsign && i < 1) return false;
    return true;
  } else {
    std::cin.clear();
    std::cin.ignore();
    return false;
  }
}
}
