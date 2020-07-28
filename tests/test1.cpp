#include <iostream>

#include "qarg.hpp"

int main([[maybe_unused]] int argc, const char *argv[]) {
  int c = 3;
  const char *v[] = {
    argv[0],
    "-y",
    "12"
  };

  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| expected output:\n";
  std::cout << "| NO ARG X\n";
  std::cout << "| y == 12\n";
  std::cout << "\n";

  qarg::parser argp;
  argp.add('x', false);
  argp.add('y', true);

  try {
    argp.parse(c, v);
  } catch (std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  auto x = argp('x');
  if (x) {
    std::cout << "x == " << *x << "\n";
  } else {
    std::cout << "NO ARG X" << "\n";
  }

  auto y = argp('y');
  if (y) {
    std::cout << "y == " << *y << "\n";
  } else {
    std::cout << "NO ARG Y" << "\n";
  }

  return 0;
}
