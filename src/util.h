#ifndef UTIL_H
#define UTIL_H

#include "main.h"

namespace util {

int positive_modulo(int i, int n);
std::string &trim(std::string &str);
std::string file_extension(const std::string &filename);
}

#endif
