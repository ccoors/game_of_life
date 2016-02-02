#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include "main.h"

namespace gol {

typedef std::vector<bool> line;
typedef std::vector<line> grid;

constexpr auto dead_cell  = " ";
constexpr auto alive_cell = "*";
constexpr int min_grid_size{4};
constexpr int default_grid_size{20};
constexpr int max_threads{32};

class Game_of_life {
public:
  Game_of_life();
  Game_of_life(const int edge_length);
  Game_of_life(const int width, const int height);

  bool valid_coords(const int x, const int y) const;
  bool valid_coords_x(const int x) const;
  bool valid_coords_y(const int y) const;
  std::pair<int, int> size() const;
  std::pair<int, int> size(const int width, const int height);
  void clear_rules();
  void add_born(const int n);
  void add_stay_alive(const int n);
  void clear();
  bool cell(const int x, const int y, const bool value);
  bool cell(const int x, const int y) const;
  bool cell_shift(const int x, const int y, const bool value);
  bool cell_shift(const int x, const int y) const;
  int threads(const int new_threads);
  int threads() const;
  bool step();

  bool load_pattern(const std::string &filename, const int x, const int y);

  int shift_coord_x(int x) const;
  int shift_coord_y(int y) const;

private:
  Game_of_life(const Game_of_life &g) = delete;

  int alive_neighbors(const int x, const int y, const grid &grid);
  void calculate_line(const int line, grid &output, const grid &old,
                      const bool thread_safe);

  friend std::ostream &operator<<(std::ostream &os, const Game_of_life &g);
  friend void thread_helper(Game_of_life &g, std::vector<int> lines,
                            grid &output, const grid &old);

  int _width, _height, _threads;
  std::mutex _grid_lock;
  grid _current_grid, _old_grid;
  std::unordered_set<int> _born, _stay_alive;
};
}

#endif
