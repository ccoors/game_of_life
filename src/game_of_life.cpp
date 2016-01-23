#include "game_of_life.h"

#include "ui.h"
#include "import.h"

namespace gol {

Game_of_life::Game_of_life() : _size{default_grid_size}, _threads{1} {
  clear();
}

Game_of_life::Game_of_life(const int new_size) : Game_of_life() {
  clear();
  size(new_size);
}

bool Game_of_life::valid_coords(const int x, const int y) const {
  return (valid_coords(x) && valid_coords(y));
}

bool Game_of_life::valid_coords(const int x) const {
  return (x >= 0 && x < _size);
}

int Game_of_life::size() const { return _size; }

int Game_of_life::size(const int new_size) {
  _size = std::max(gol::min_grid_size, new_size);
  clear();
  return _size;
}

bool Game_of_life::cell(const int x, const int y, const bool value) {
  if (!valid_coords(x, y)) {
    return false;
  }

  _current_grid[y][x] = value;
  return true;
}

bool Game_of_life::cell(const int x, const int y) const {
  if (!valid_coords(x, y)) {
    return false;
  }

  return _current_grid[y][x];
}

bool Game_of_life::cell_shift(const int x, const int y, const bool value) {
  _current_grid[shift_coord(y)][shift_coord(x)] = value;
  return true;
}

bool Game_of_life::cell_shift(const int x, const int y) const {
  return _current_grid[shift_coord(y)][shift_coord(x)];
}

int Game_of_life::threads(const int new_threads) {
  _threads = std::max(1, std::min(max_threads, new_threads));
  return _threads;
}

int Game_of_life::threads() const { return _threads; }

void Game_of_life::clear() {
  _current_grid.clear();
  _old_grid.clear();
  for (auto i = 0; i < _size; i++) {
    _current_grid.emplace_back(_size, false);
    _old_grid.emplace_back(_size, false);
  }
}

int Game_of_life::shift_coord(int i) const {
  while (i < 0) {
    i += _size;
  }

  while (i >= _size) {
    i -= _size;
  }

  return i;
}

int Game_of_life::alive_neighbors(const int x, const int y, const grid &grid) {
  int n{0};
  for (int delta_y = -1; delta_y < 2; delta_y++) {
    for (int delta_x = -1; delta_x < 2; delta_x++) {
      if (!(delta_y == 0 && delta_x == 0)) {
        int ry = shift_coord(y + delta_y);
        int rx = shift_coord(x + delta_x);

        n += (grid[ry][rx] ? 1 : 0);
      }
    }
  }
  return n;
}

void Game_of_life::calculate_line(const int line, grid &output, const grid &old,
                                  const bool thread_safe) {
  for (int x = 0; x < _size; x++) {
    int n = alive_neighbors(x, line, old);
    bool new_state{false};
    if (n == 2) {
      new_state = old[line][x];
    } else if (n == 3) {
      new_state = true;
    }

    if (thread_safe) {
      std::lock_guard<std::mutex> lock{_grid_lock};
      output[line][x] = new_state;
    } else {
      output[line][x] = new_state;
    }
  }
}

void thread_helper(Game_of_life &g, int line, grid &output, const grid &old) {
  g.calculate_line(line, output, old, true);
}

void Game_of_life::step() {
  std::swap(_current_grid, _old_grid);
  if (_threads < 2) {
    for (int y = 0; y < _size; y++) {
      calculate_line(y, _current_grid, _old_grid, false);
    }
  } else {
    int current_line{0};
    std::vector<std::thread> running{};
    while (current_line < _size) {
      for (int i = 0; i < _threads; i++) {
        running.emplace_back(thread_helper, std::ref(*this), current_line,
                             std::ref(_current_grid), std::ref(_old_grid));
        current_line++;
        if (current_line >= _size) {
          break;
        }
      }

      for (auto &t : running) {
        t.join();
      }
      running.clear();
    }
  }
}

bool Game_of_life::load_pattern(const std::string filename, const int x,
                                const int y) {
  std::ifstream in{filename, std::ifstream::in};
  if (in.is_open()) {
    grid backup = _current_grid;
    bool ok{false};

    if (filename.substr(filename.find_last_of(".") + 1) == "rle") {
      ok = import::import_rle(in, *this, x, y);
    }

    if (!ok) {
      _current_grid = backup;
      std::cout << "File could not be interpreted.\n";
    }

    return ok;
  }
  std::cout << "Could not open file.\n";

  return false;
}

std::ostream &operator<<(std::ostream &os, const Game_of_life &g) {
  os << "+" << ui::repeat(g._size, "-") << "+\n";
  for (auto y : g._current_grid) {
    os << "|";
    for (auto x : y) {
      os << (x ? alive_cell : dead_cell);
    }
    os << "|\n";
  }
  os << "+" << ui::repeat(g._size, "-") << "+\n";
  return os;
}
} // namespace gol
