#include <iostream>
#include <string_view>

#include "qarg.hpp"

constexpr std::string_view expected = "-y is a required argument";

int main([[maybe_unused]] int argc, const char *argv[]) {
  int c = 3;
  const char *v[] = {
    argv[0],
    "-x",
    "6.9"
  };

  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| expected output:\n";
  std::cout << expected;
  std::cout << "\n\n";

  qarg::parser argp;
  argp.add<bool>('h', "display this text");
  argp.add<bool>('v', "verbose");
  argp.add<int>('x', "x coord", true);
  argp.add<int>('y', "y coord", true);

  try {
    argp.parse(c, v);
  } catch (std::invalid_argument &e) {
    std::cerr << e.what() << "\n";

    if (e.what() == expected) {
      return 0;
    }

    return 1;
  }

  return 1;
}
