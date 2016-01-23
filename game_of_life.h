#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include "main.h"

namespace gol {

typedef std::vector<bool> line;
typedef std::vector<line> grid;

constexpr auto dead_cell  = " ";
constexpr auto alive_cell = "*";
constexpr int min_grid_size{4};
constexpr int default_grid_size{10};
constexpr int max_threads{32};

class Game_of_life {
public:
  Game_of_life();
  Game_of_life(const int new_size);

  bool valid_coords(const int x, const int y) const;
  bool valid_coords(const int x) const;
  int size() const;
  int size(const int new_size);
  void clear();
  bool cell(const int x, const int y, const bool value);
  bool cell(const int x, const int y) const;
  int threads(const int new_threads);
  int threads() const;
  void step();

  int shift_coord(int i) const;

private:
  Game_of_life(const Game_of_life& g);
  int alive_neighbors(const int x, const int y, const grid& grid);
  void calculate_line(const int line, grid& output, const grid& old,
                      const bool thread_safe);

  friend std::ostream& operator<<(std::ostream& os, const Game_of_life& g);
  friend void thread_helper(Game_of_life& g, int line, grid& output,
                            const grid& old);

  int _size, _threads;
  std::mutex _grid_lock;
  grid _current_grid, _old_grid;
};
}

#endif
