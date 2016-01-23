#include "commands.h"

namespace commands {

std::random_device random_device;
std::default_random_engine random_engine{random_device()};

std::vector<command_structure> commands = {
    {"?", "?", "?", &help, "Print this help"},
    {"print", "(p)rint", "p", &print_grid, "Print current grid"},
    {"clear", "(c)lear", "c", &clear_grid, "Clear the grid"},
    {"resize", "resi(z)e n", "z", &resize_grid,
     "Set grid size to n x n and clear"},
    {"set", "(s)et x y", "s", &set_cell, "Set cell at x y alive"},
    {"unset", "(u)nset x y", "u", &unset_cell, "Set cell at x y dead"},
    {"fill", "(f)ill n", "f", &fill_grid, "Set up to n cells alive"},
    {"threads", "(t)hreads n", "t", &set_threads, "Set the amount of threads"},
    {"glider", "(g)lider x y", "g", &create_glider, "Place a glider at x y"},
    {"run", "(r)un n", "r", &run, "Run n iterations"},
    {"step", "st(e)p", "e", &step, "Run 1 iteration"},
    {"exit", "exit", "", &pexit, "Exit"}};

constexpr int max_command_width{14};

bool command(std::string command, gol::Game_of_life& g) {
  for (auto c : commands) {
    if (command == c.name || command == c.shortcut) return c.function(g);
  }

  std::cout << "Unknown command.\n";
  return true;
}

void print_command_name(const std::string& name) {
  std::cout.width(max_command_width);
  std::cout << name;
}

bool help(gol::Game_of_life& g) {
  std::cout << " === Help ===\n";
  for (auto c : commands) {
    print_command_name(c.help);
    std::cout << " | " << c.description << "\n";
  }

  std::cout << "\nCoordinates are 0-based. (0 0) is top left corner.\n";
  int hardware_threads = std::thread::hardware_concurrency();
  if (hardware_threads > 1)
    std::cout << "Use as many threads as possible (up to " << gol::max_threads
              << ")\nif you have more than one CPU core.\nYour CPU supports "
              << hardware_threads << " hardware threads.\n";

  std::cout << "\n";
  return true;
}

bool print_grid(gol::Game_of_life& g) {
  std::cout << g;
  return true;
}

bool clear_grid(gol::Game_of_life& g) {
  g.clear();
  return true;
}

bool resize_grid(gol::Game_of_life& g) {
  int size;
  if (ui::input_int(size, true)) {
    g.size(size);
    std::cout << "New grid size is " << g.size() << ".\n";
  } else {
    std::cout << "Invalid grid size entered.\n";
  }
  return true;
}

void input_set_cell(gol::Game_of_life& g, bool value) {
  int x, y;
  if (ui::input_int(x) && ui::input_int(y)) {
    if (!g.cell(x, y, value)) {
      std::cout << "Invalid coordinates.\n";
    }
  } else {
    std::cout << "Could not parse coordinates.\n";
  }
}

bool set_cell(gol::Game_of_life& g) {
  input_set_cell(g, true);

  return true;
}

bool unset_cell(gol::Game_of_life& g) {
  input_set_cell(g, false);

  return true;
}

bool fill_grid(gol::Game_of_life& g) {
  int n;
  if (ui::input_int(n, true)) {
    std::uniform_int_distribution<int> grid_distribution(0, g.size() - 1);
    for (int i = 0; i < n; i++) {
      int x = grid_distribution(random_engine);
      int y = grid_distribution(random_engine);

      g.cell(x, y, true);
    }
  } else {
    std::cout << "Invalid number.\n";
  }
  return true;
}

bool set_threads(gol::Game_of_life& g) {
  int n;
  if (ui::input_int(n, true)) {
    g.threads(n);
    std::cout << "Set to " << g.threads() << " threads.\n";
  } else {
    std::cout << "Invalid number.\n";
  }
  return true;
}

bool create_glider(gol::Game_of_life& g) {
  int x, y;
  if (ui::input_int(x) && ui::input_int(y)) {
    g.cell(g.shift_coord(x + 1), g.shift_coord(y), true);
    g.cell(g.shift_coord(x + 2), g.shift_coord(y + 1), true);
    g.cell(g.shift_coord(x + 2), g.shift_coord(y + 2), true);
    g.cell(g.shift_coord(x + 1), g.shift_coord(y + 2), true);
    g.cell(g.shift_coord(x), g.shift_coord(y + 2), true);
  } else {
    std::cout << "Invalid coordinates.\n";
  }
  return true;
}

bool run(gol::Game_of_life& g) {
  int n;
  if (ui::input_int(n, true)) {
    for (int i = 0; i < n; i++) {
      g.step();
      std::cout << "Step " << (i + 1) << ":\n";
      std::cout << g << std::endl;
    }
  } else {
    std::cout << "Invalid number of steps.\n";
  }
  return true;
}

bool step(gol::Game_of_life& g) {
  g.step();
  std::cout << g << std::endl;
  return true;
}

bool pexit(gol::Game_of_life& g) { return false; }
}
