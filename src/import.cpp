#include "import.h"

namespace import {

std::string &trim(std::string &str) {
  str.erase(str.begin(), find_if(str.begin(), str.end(), [](char &ch) -> bool {
              return !isspace(ch);
            }));
  str.erase(find_if(str.rbegin(), str.rend(), [](char &ch) -> bool {
              return !isspace(ch);
            }).base(), str.end());
  return str;
}

/**
 * TODO: Make this less ugly. :/
*/
bool import_rle(std::ifstream &i, gol::Game_of_life &g, const int x,
                const int y) {
  std::string line;
  std::stringstream stringstr;
  int width{0}, height{0};
  bool data{false};
  int local_x{0}, local_y{0};

  while (i.good()) {
    std::getline(i, line);

    if (line[0] != '#' && trim(line) != "") {
      stringstr.clear();
      stringstr.str(line);
      std::string xs, ys, equals[2], comma;
      int values[2];

      if (!data && stringstr >> xs && stringstr >> equals[0] &&
          stringstr >> values[0] && stringstr >> comma && stringstr >> ys &&
          stringstr >> equals[1] && stringstr >> values[1] && xs == "x" &&
          equals[0] == "=" && comma == "," && ys == "y" && equals[1] == "=") {
        width  = values[0];
        height = values[1];
        data   = true;
      } else {
        if (!data) {
          return false;
        }
        while (stringstr.good()) {
          int count{0};
          char c{0};
          if (stringstr >> count && stringstr.get(c)) {
            for (int lx = 0; lx < count; lx++) {
              if (c == '$') {
                local_x = 0;
                local_y++;
                continue;
              }
              g.cell_shift(x + local_x, y + local_y, (c == 'o'));
              local_x++;
              if (local_x > width) {
                local_x = 0;
                local_y++;
              }
            }
          } else {
            stringstr.clear();
            if (stringstr.get(c)) {
              if (c == 'b' || c == 'o') {
                g.cell_shift(x + local_x, y + local_y, (c == 'o'));
                local_x++;
                if (local_x > width) {
                  local_x = 0;
                  local_y++;
                }
              } else if (c == '$') {
                local_x = 0;
                local_y++;
              } else if (c == '!') {
                return true;
              }
            } else {
              stringstr.clear();
              std::string remaining;
              getline(stringstr, remaining);
              if (remaining != "") {
                std::cout << "Unknown char in file. Remaining: " << remaining
                          << "\n";
                return false;
              }
            }
          }
        }
      }
    }
  }
  return true;
}
} // namespace import
