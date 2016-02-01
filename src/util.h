#ifndef UTIL_H
#define UTIL_H

#include "main.h"

namespace util {

struct No_rule_exception {};

int positive_modulo(int i, int n);
std::string &trim(std::string &str);
std::string file_extension(const std::string &filename);
bool digit_string(const std::string &s);
std::string string_pair(const std::pair<int, int> t);
}

#endif
