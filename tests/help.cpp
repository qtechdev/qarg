#include <iostream>
#include <string_view>

#include "qarg.hpp"

constexpr std::string_view expected = (
  "options:\n"
  "-h            display this text\n"
  "-v            verbose\n"
  "-x <int>      x coord\n"
  "-y <int>      y coord\n"
);


int main([[maybe_unused]] int argc, const char *argv[]) {
  int c = 2;
  const char *v[] = {
    argv[0],
    "-h"
  };

  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| expected output:\n";
  std::cout << expected;
  std::cout << "\n";

  qarg::parser argp;
  argp.add<bool>('h', "display this text");
  argp.add<bool>('v', "verbose");
  argp.add<int>('x', "x coord", true);
  argp.add<int>('y', "y coord", true);

  try {
    argp.parse(c, v);
  } catch (std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  if (argp('h')) {
    std::cout << argp.help();

    if (argp.help() == expected) {
      return 0;
    }
  }

  return 1;
}
