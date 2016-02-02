#include "import_export.h"

namespace import_export {

bool next_cell(int &x, int &y, int &width, int &height) {
  x++;
  if (x > width) {
    x = 0;
    y++;
    return !(y > height);
  }
  return true;
}

std::pair<std::string, std::string> get_rules(const std::string &s) {
  std::string born{}, stay_alive{};

  try {
    born       = s.substr(0, s.find_first_of("/"));
    stay_alive = s.substr(s.find_first_of("/") + 1);

    if (stay_alive[0] == 'B' && born[0] == 'S') std::swap(stay_alive, born);
    born       = born.substr(1);
    stay_alive = stay_alive.substr(1);
  } catch (const std::out_of_range &e) {
    throw util::No_rule_exception{};
  }

  util::trim(born);
  util::trim(stay_alive);

  if (!util::digit_string(born) || !util::digit_string(stay_alive)) {
    throw util::No_rule_exception{};
  }

  return std::make_pair(born, stay_alive);
}

bool import_rle(std::istream &i, gol::Game_of_life &g, const int x,
                const int y) {
  std::string line, xs, ys, equals[3], comma[2], rule, rulestring;
  std::stringstream stringstr;
  int width{0}, height{0}, local_x{0}, local_y{0}, values[2];
  bool data{false};

  while (i.good()) {
    std::getline(i, line);

    if (!line.length() || util::trim(line) == "" || line[0] == '#') continue;

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
        stringstr >> values[0] && stringstr >> comma[0] && stringstr >> ys &&
        stringstr >> equals[1] && stringstr >> values[1] && xs == "x" &&
        equals[0] == "=" && comma[0] == "," && ys == "y" && equals[1] == "=") {
      width  = values[0];
      height = values[1];
      data   = true;

      if (stringstr >> comma[1] && stringstr >> rule &&
          stringstr >> equals[2] && stringstr >> rulestring &&
          comma[1] == "," && rule == "rule" && equals[2] == "=") {
        std::cout << "Rule definition found in file.\n";
        try {
          auto pair = get_rules(rulestring);
          g.clear_rules();
          for (char c : pair.first) {
            g.add_born(c - '0');
          }
          for (char c : pair.second) {
            g.add_stay_alive(c - '0');
          }
          std::cout << "Rules changed to B" << pair.first << "/S" << pair.second
                    << ".\n";
        } catch (const util::No_rule_exception &e) {
          std::cout << "WARNING: Invalid rules defined in file. Ignoring.\n";
        }
      } else {
        std::cout << "WARNING: No rule definition in file.\n";
      }
    }
  }
  return true;
}

bool export_cells(const std::string &filename, const gol::Game_of_life &g) {
  std::ofstream file{filename};
  if (file.is_open()) {
    file << "!Name: Export\n";
    auto size = g.size();
    for (int y = 0; y < size.first; y++) {
      for (int x = 0; x < size.second; x++) {
        file << (g.cell(x, y) ? "O" : ".");
      }
      file << "\n";
    }
    file.close();
    return true;
  }
  return false;
}

bool import_cells(std::istream &i, gol::Game_of_life &g, const int x,
                  const int y) {
  std::string line;
  std::stringstream stringstr;
  int local_x{0}, local_y{0};
  char c;

  while (i.good()) {
    std::getline(i, line);

    if (!line.length() || util::trim(line) == "") {
      local_y++;
      continue;
    }

    if (line[0] == '!') {
      continue;
    }

    stringstr.clear();
    stringstr.str(line);
    while (stringstr.good()) {
      stringstr.get(c);
      if (!stringstr.good()) {
        local_x = 0;
        local_y++;
        continue;
      }

      switch (c) {
      case '.':
        g.cell_shift(x + local_x, y + local_y, false);
        break;
      case 'O':
        g.cell_shift(x + local_x, y + local_y, true);
        break;
      default:
        std::cout << "Unexpected character in file (" << +c << "). Aborting.\n";
        return false;
      }
      local_x++;
    }
  }
  return true;
}

bool import_life(std::istream &i, gol::Game_of_life &g, const int x,
                 const int y) {
  std::string line;
  std::string life_version;
  std::stringstream stringstr;
  std::vector<std::pair<int, int>> alive_cells;
  int linenumber{0}, version{0}, min_x{0}, min_y{0};

  while (i.good()) {
    std::getline(i, line);
    linenumber++;

    if (!line.length() || util::trim(line) == "") {
      continue;
    }

    if (line[0] == '#' && linenumber > 0) {
      continue;
    }

    if (linenumber == 1) {
      life_version = line;

      // Enable when implementing support for Life 1.05
      // version = (life_version == "#Life 1.05") ? 5 : 0;
      version = (life_version == "#Life 1.06") ? 6 : 0;

      if (!version) {
        std::cout << "Unsupported version (" << life_version
                  << "). Aborting.\n";
        return false;
      }
      continue;
    }

    stringstr.clear();
    stringstr.str(line);
    int cx, cy;
    if (!(stringstr >> cx) || !(stringstr >> cy)) return false;
    alive_cells.emplace_back(cx, cy);
    min_x = std::min(min_x, cx);
    min_y = std::min(min_y, cy);
  }

  for (auto &p : alive_cells) {
    g.cell_shift(p.first + x - min_x, p.second + y - min_y, true);
  }

  return true;
}

} // namespace import
