#include <iostream>
#include <sstream>
#include <string_view>

#include "qarg.hpp"

constexpr std::string_view expected = (
  "NO ARG X\n"
  "y == 12\n"
);

int main([[maybe_unused]] int argc, const char *argv[]) {
  int c = 3;
  const char *v[] = {
    argv[0],
    "-y",
    "12"
  };

  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| expected output:\n";
  std::cout << expected;
  std::cout << "\n";

  qarg::parser argp;
  argp.add<int>('x', "x coord");
  argp.add<int>('y', "y coord", true);

  try {
    argp.parse(c, v);
  } catch (std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  std::stringstream ss;

  auto x = argp('x');
  if (x) {
    ss << "x == " << *x << "\n";
  } else {
    ss << "NO ARG X\n";
  }

  auto y = argp('y');
  if (y) {
    ss << "y == " << *y << "\n";
  } else {
    ss << "NO ARG Y\n";
  }

  std::cout << ss.str();

  if (ss.str() == expected) {
    return 0;
  }

  return 1;
}
