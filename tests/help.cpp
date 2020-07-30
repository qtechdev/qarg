#include <iostream>

#include "qarg.hpp"

int main([[maybe_unused]] int argc, const char *argv[]) {
  int c = 2;
  const char *v[] = {
    argv[0],
    "-h"
  };

  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| expected output:\n";
  std::cout << "| options:\n";
  std::cout << "| -h            display this text\n";
  std::cout << "| -v            verbose\n";
  std::cout << "| -x <int>      x coord\n";
  std::cout << "| -y <int>      y coord\n";
  std::cout << "\n\n";

  qarg::parser argp;
  argp.add<bool>('h', false, "display this text");
  argp.add<bool>('v', false, "verbose");
  argp.add<int>('x', true, "x coord");
  argp.add<int>('y', true, "y coord");

  try {
    argp.parse(c, v);
  } catch (std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  if (argp('h')) {
    std::cout << argp.help() << "\n";
    return 0;
  }

  return 0;
}
