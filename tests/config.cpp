#include <iostream>

#include "qarg.hpp"

constexpr std::string_view expected = (
  "v == true\n"
  "x == 1234\n"
  "y == 5678\n"
);

int main([[maybe_unused]] int argc, const char *argv[]) {
  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| expected output:\n";
  std::cout << expected;
  std::cout << "\n";

  int c = 3;
  const char *v[] = {
    argv[0],
    "-c",
    "config.json"
  };

  qarg::parser argp;
  argp.add<bool>('h', "display this text");
  argp.add<bool>('v', "verbose");
  argp.add<int>('x', "x coord", true);
  argp.add<int>('y', "y coord", true);
  argp.config('c', "config file");

  try {
    argp.parse(c, v);
  } catch (std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  if (*argp.get<bool>('h')) {
    std::cout << argp.help();
  }

  std::stringstream ss;
  ss << "v == " << std::boolalpha << *argp.get<bool>('v') << "\n";
  ss << "x == " << *argp.get<int>('x') << "\n";
  ss << "y == " << *argp.get<int>('y') << "\n";

  std::cout << ss.str();

  if (ss.str() == expected) {
    return 0;
  }

  return 1;
}
