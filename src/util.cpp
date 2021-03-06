#include "util.h"

namespace util {

int positive_modulo(int i, int n) { return (i % n + n) % n; }

std::string &trim(std::string &str) {
  str.erase(str.begin(), find_if(str.begin(), str.end(), [](char &ch) -> bool {
              return !isspace(ch);
            }));
  str.erase(find_if(str.rbegin(), str.rend(), [](char &ch) -> bool {
              return !isspace(ch);
            }).base(), str.end());
  return str;
}

std::string file_extension(const std::string &filename) {
  return filename.substr(filename.find_last_of(".") + 1);
}

bool digit_string(const std::string &s) {
  for (auto &c : s) {
    if (!isdigit(c)) return false;
  }
  return true;
}

std::string string_pair(const std::pair<int, int> t) {
  return std::to_string(t.first) + " x " + std::to_string(t.second);
}

} // namespace util
