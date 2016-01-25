#include "commands.h"

namespace commands {

std::random_device random_device;
std::default_random_engine random_engine{random_device()};

int hardware_threads = std::thread::hardware_concurrency();

bool verbose_grid{true};
int delay_ms{50};

// clang-format off
std::vector<command_structure> commands = {
  {"?",       "?",               "?", &help,          "Print this help"},
  {"print",   "(p)rint",         "p", &print_grid,    "Print current grid"},
  {"clear",   "(c)lear",         "c", &clear_grid,    "Clear the grid"},
  {"resize",  "resi(z)e n",      "z", &resize_grid,   "Set grid size to n x n and clear"},
  {"set",     "(s)et x y",       "s", &set_cell,      "Set cell at x y alive"},
  {"unset",   "(u)nset x y",     "u", &unset_cell,    "Set cell at x y dead"},
  {"fill",    "(f)ill n",        "f", &fill_grid,     "Set up to n cells alive"},
  {"threads", "(t)hreads n",     "t", &set_threads,   "Set the amount of threads"},
  {"glider",  "(g)lider x y",    "g", &create_glider, "Place a glider at x y"},
  {"load",    "(l)oad file x y", "l", &load_pattern,  "Load pattern from file to x y"},
  {"quiet",   "(q)uiet",         "q", &quiet,         "Disable output during running"},
  {"verbose", "(v)erbose",       "v", &verbose,       "Enable output during running"},
  {"delay",   "(d)elay n",       "d", &delay,         "Wait for n milliseconds after each step"},
  {"run",     "(r)un n",         "r", &run,           "Run n iterations"},
  {"step",    "st(e)p",          "e", &step,          "Run 1 iteration"},
  {"exit",    "exit",            "",  &pexit,         "Exit"}
};
// clang-format on

constexpr int max_command_width{16};

bool command(std::string command, gol::Game_of_life &g) {
  for (auto c : commands) {
    if (command == c.name || command == c.shortcut) {
      return c.function(g);
    }
  }

  std::cout << "Unknown command.\n";
  return true;
}

void print_command_name(const std::string &name) {
  std::cout.width(max_command_width);
  std::cout << name;
}

bool quiet(gol::Game_of_life &g) {
  verbose_grid = false;
  return true;
}

bool verbose(gol::Game_of_life &g) {
  verbose_grid = true;
  return true;
}

bool delay(gol::Game_of_life &g) {
  int n;
  if (ui::input_int(n, true, true))
    delay_ms = n;
  else
    std::cout << "Invalid value.\n";

  return true;
}

bool help(gol::Game_of_life &g) {
  std::cout << " === Help ===\n";
  for (auto c : commands) {
    print_command_name(c.help);
    std::cout << " | " << c.description << "\n";
  }

  std::cout << "\nCoordinates are 0-based. (0 0) is top left corner.\n";
  std::cout << "\nCurrent the following file formats are supported:\n";
  std::cout << " * Life 1.06 (.lif or .life)\n";
  std::cout << " * Plaintext (.cells)\n";
  std::cout << " * RLE (.rle)\n";
  std::cout << "Please make sure the grid is large enough before loading.\n";

  if (hardware_threads > 1) {
    std::cout << "\nUse as many threads as possible (up to " << gol::max_threads
              << ")\nif you have more than one CPU core.\nYour CPU supports "
              << hardware_threads << " hardware threads.\n";
  }

  std::cout << "\n";
  return true;
}

bool print_grid(gol::Game_of_life &g) {
  std::cout << g;
  return true;
}

bool clear_grid(gol::Game_of_life &g) {
  g.clear();
  std::cout << "Grid cleared.\n";
  return true;
}

bool resize_grid(gol::Game_of_life &g) {
  int size;
  if (ui::input_int(size, true, false)) {
    g.size(size);
    std::cout << "New grid size is " << g.size() << ".\n";
  } else {
    std::cout << "Invalid grid size entered.\n";
  }
  return true;
}

void input_set_cell(gol::Game_of_life &g, bool value) {
  int x, y;
  if (ui::input_int(x, true, true) && ui::input_int(y, true, true)) {
    if (!g.cell(x, y, value)) {
      std::cout << "Invalid coordinates.\n";
    }
  } else {
    std::cout << "Could not parse coordinates.\n";
  }
}

bool set_cell(gol::Game_of_life &g) {
  input_set_cell(g, true);

  return true;
}

bool unset_cell(gol::Game_of_life &g) {
  input_set_cell(g, false);

  return true;
}

bool fill_grid(gol::Game_of_life &g) {
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

bool set_threads(gol::Game_of_life &g) {
  int n;
  if (ui::input_int(n, true)) {
    g.threads(n);
    std::cout << "Set to " << g.threads() << " threads.\n";
  } else {
    std::cout << "Invalid number.\n";
  }
  return true;
}

bool create_glider(gol::Game_of_life &g) {
  int x, y;
  if (ui::input_int(x, false, true) && ui::input_int(y, false, true)) {
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

bool load_pattern(gol::Game_of_life &g) {
  std::string filename{""};
  int x, y;

  if (std::cin >> filename && ui::input_int(x, true, true) &&
      ui::input_int(y, true, true)) {
    if (g.load_pattern(filename, x, y)) {
      std::cout << "OK\n";
    }
  } else {
    std::cin.clear();
    std::cin.ignore();
    std::cout << "Invalid parameters.\n";
  }
  return true;
}

bool run(gol::Game_of_life &g) {
  int n;
  if (ui::input_int(n, true)) {
    for (int i = 0; i < n; i++) {
      if (!g.step()) {
        std::cout << "Aborted after " << (i + 1) << " step"
                  << (i > 0 ? "s" : "")
                  << " because of no change comprared do previous step.\n";
        break;
      };
      if (verbose_grid) {
        std::cout << "Step " << (i + 1) << ":\n";
        std::cout << g << std::endl;
        if (delay_ms) {
          std::chrono::milliseconds wait_for(delay_ms);
          std::this_thread::sleep_for(wait_for);
        }
      }
    }
  } else {
    std::cout << "Invalid number of steps.\n";
  }
  return true;
}

bool step(gol::Game_of_life &g) {
  g.step();
  if (verbose_grid) std::cout << g << std::endl;
  return true;
}

bool pexit(gol::Game_of_life &g) { return false; }
} // namespace commands
