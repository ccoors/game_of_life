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

bool next_cell(int &x, int &y, int &width, int &height) {
  x++;
  if (x > width) {
    x = 0;
    y++;
    return !(y > height);
  }
  return true;
}

bool import_rle(std::ifstream &i, gol::Game_of_life &g, const int x,
                const int y) {
  std::string line, xs, ys, equals[2], comma;
  std::stringstream stringstr;
  int width{0}, height{0}, local_x{0}, local_y{0}, values[2];
  bool data{false};

  while (i.good()) {
    std::getline(i, line);

    if (!line.length() || trim(line) == "" || line[0] == '#') continue;

    stringstr.clear();
    stringstr.str(line);

    while (stringstr.good() && data) {
      int count{0};
      char c{0};

      if (!(stringstr >> count)) {
        stringstr.clear();
        count = 1;
      }

      stringstr.get(c);
      if (!stringstr.good()) continue;
      for (int lx = 0; lx < count; lx++) {
        switch (c) {
        case 'b':
          g.cell_shift(x + local_x, y + local_y, false);
          next_cell(local_x, local_y, width, height);
          break;
        case 'o':
          g.cell_shift(x + local_x, y + local_y, true);
          next_cell(local_x, local_y, width, height);
          break;
        case '$':
          local_x = 0;
          local_y++;
          break;
        case '!':
          return true;
        default:
          std::cout << "Unexpected character in file (" << +c
                    << "). Aborting.\n";
          return false;
        }
      }
    }

    if (!data && stringstr >> xs && stringstr >> equals[0] &&
        stringstr >> values[0] && stringstr >> comma && stringstr >> ys &&
        stringstr >> equals[1] && stringstr >> values[1] && xs == "x" &&
        equals[0] == "=" && comma == "," && ys == "y" && equals[1] == "=") {
      width  = values[0];
      height = values[1];
      data   = true;
    }
  }
  return true;
}

} // namespace import
