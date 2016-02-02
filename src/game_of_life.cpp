#include "game_of_life.h"

#include "util.h"
#include "ui.h"
#include "import_export.h"

namespace gol {

Game_of_life::Game_of_life()
    : _width{default_grid_size}, _height{default_grid_size}, _threads{1},
      _born{}, _stay_alive{} {
  add_born(3);
  add_stay_alive(2);
  add_stay_alive(3);
  clear();
}

Game_of_life::Game_of_life(const int edge_length) : Game_of_life() {
  size(edge_length, edge_length);
}

Game_of_life::Game_of_life(const int width, const int height) : Game_of_life() {
  size(width, height);
}

bool Game_of_life::valid_coords(const int x, const int y) const {
  return (valid_coords_x(x) && valid_coords_y(y));
}

bool Game_of_life::valid_coords_x(const int x) const {
  return (x >= 0 && x < _width);
}

bool Game_of_life::valid_coords_y(const int y) const {
  return (y >= 0 && y < _height);
}

std::pair<int, int> Game_of_life::size() const {
  return std::make_pair(_width, _height);
}

std::pair<int, int> Game_of_life::size(const int width, const int height) {
  _width  = std::max(gol::min_grid_size, width);
  _height = std::max(gol::min_grid_size, height);
  clear();
  return size();
}

void Game_of_life::clear_rules() {
  _born.clear();
  _stay_alive.clear();
}

void Game_of_life::add_born(const int n) {
  if (n < 0 || n > 8) return;
  _born.insert(n);
}

void Game_of_life::add_stay_alive(const int n) {
  if (n < 0 || n > 8) return;
  _stay_alive.insert(n);
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
  _current_grid[shift_coord_y(y)][shift_coord_x(x)] = value;
  return true;
}

bool Game_of_life::cell_shift(const int x, const int y) const {
  return _current_grid[shift_coord_y(y)][shift_coord_x(x)];
}

int Game_of_life::threads(const int new_threads) {
  _threads = std::max(1, std::min(max_threads, new_threads));
  return _threads;
}

int Game_of_life::threads() const { return _threads; }

void Game_of_life::clear() {
  _current_grid.clear();
  _old_grid.clear();
  for (auto i = 0; i < _height; i++) {
    _current_grid.emplace_back(_width, false);
    _old_grid.emplace_back(_width, false);
  }
}

int Game_of_life::shift_coord_x(int x) const {
  while (x < 0) {
    x += _width;
  }

  while (x >= _width) {
    x -= _width;
  }

  return x;
}

int Game_of_life::shift_coord_y(int y) const {
  while (y < 0) {
    y += _height;
  }

  while (y >= _height) {
    y -= _height;
  }

  return y;
}

int Game_of_life::alive_neighbors(const int x, const int y, const grid &grid) {
  int n{0};
  for (int delta_y = -1; delta_y < 2; delta_y++) {
    for (int delta_x = -1; delta_x < 2; delta_x++) {
      if (!(delta_y == 0 && delta_x == 0)) {
        int ry = shift_coord_y(y + delta_y);
        int rx = shift_coord_x(x + delta_x);

        n += (grid[ry][rx] ? 1 : 0);
      }
    }
  }
  return n;
}

void Game_of_life::calculate_line(const int line, grid &output, const grid &old,
                                  const bool thread_safe) {
  for (int x = 0; x < _width; x++) {
    int n = alive_neighbors(x, line, old);
    bool current_state{old[line][x]};
    bool new_state{false};

    if (current_state) {
      auto f = _stay_alive.find(n);
      if (f != _stay_alive.end()) new_state = true;
    } else {
      auto f = _born.find(n);
      if (f != _born.end()) new_state = true;
    }

    if (thread_safe) {
      std::lock_guard<std::mutex> lock{_grid_lock};
      output[line][x] = new_state;
    } else {
      output[line][x] = new_state;
    }
  }
}

void thread_helper(Game_of_life &g, std::vector<int> lines, grid &output,
                   const grid &old) {
  for (auto &l : lines) {
    g.calculate_line(l, output, old, true);
  }
}

bool Game_of_life::step() {
  std::swap(_current_grid, _old_grid);
  if (_threads < 2) {
    for (int y = 0; y < _height; y++) {
      calculate_line(y, _current_grid, _old_grid, false);
    }
  } else {
    std::vector<std::thread> running{};
    std::map<int, std::vector<int>> to_calculate{};

    for (int current_line = 0; current_line < _height; current_line++) {
      to_calculate[util::positive_modulo(current_line, _threads)].push_back(
          current_line);
    }

    for (auto &l : to_calculate) {
      running.emplace_back(thread_helper, std::ref(*this), std::ref(l.second),
                           std::ref(_current_grid), std::ref(_old_grid));
    }

    for (auto &t : running) {
      t.join();
    }
    running.clear();
  }
  return !std::equal(_current_grid.begin(), _current_grid.end(),
                     _old_grid.begin());
}

bool Game_of_life::load_pattern(const std::string &filename, const int x,
                                const int y) {
  std::ifstream in{filename, std::ifstream::in};
  if (in.is_open()) {
    grid backup = _current_grid;
    bool ok{false};
    std::string extension = util::file_extension(filename);

    if (extension == "rle") {
      ok = import_export::import_rle(in, *this, x, y);
    } else if (extension == "cells") {
      ok = import_export::import_cells(in, *this, x, y);
    } else if (extension == "lif" || extension == "life") {
      ok = import_export::import_life(in, *this, x, y);
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
  os << "+" << ui::repeat(g._width, "-") << "+\n";
  for (auto &y : g._current_grid) {
    os << "|";
    for (auto x : y) {
      os << (x ? alive_cell : dead_cell);
    }
    os << "|\n";
  }
  os << "+" << ui::repeat(g._width, "-") << "+\n";
  return os;
}
} // namespace gol
