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

} // namespace util