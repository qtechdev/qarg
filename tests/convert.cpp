#include <iostream>
#include <sstream>
#include <string_view>

#include "qarg.hpp"

constexpr std::string_view expected = (
  "h == false\n"
  "v == true\n"
  "(x, y) == (6, 4)\n"
);

int main([[maybe_unused]] int argc, const char *argv[]) {
  int c = 5;
  const char *v[] = {
    argv[0],
    "-vx",
    "6.9",
    "-y",
    "4.20"
  };

  std::cout << "+----------------------------------------------------------\n";
  std::cout << "| expected output:\n";
  std::cout << "| h == false\n";
  std::cout << "| v == true\n";
  std::cout << "| (x, y) == (6, 4)\n";

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
    return 1;
  }

  auto arg_h = argp.get<bool>('h');
  auto arg_v = argp.get<bool>('v');
  auto arg_x = argp.get<int>('x');
  auto arg_y = argp.get<int>('y');

  std::stringstream ss;
  ss << "h == " << std::boolalpha << *arg_h << "\n";
  ss << "v == " << std::boolalpha << *arg_v << "\n";
  ss << "(x, y) == (";
  ss << (arg_x ? *arg_x : -1) << ", " << (arg_y ? *arg_y : -1);
  ss << ")\n";

  std::cout << ss.str();

  if (ss.str() == expected) {
    return 0;
  }

  return 1;
}
