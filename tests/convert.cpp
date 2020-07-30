#include <iostream>

#include "qarg.hpp"

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
  std::cout << "| (x, y) == (6, 9)\n";

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

  auto arg_h = argp.get<bool>('h');
  auto arg_v = argp.get<bool>('v');
  auto arg_x = argp.get<int>('x');
  auto arg_y = argp.get<int>('y');

  std::cout << "h == " << std::boolalpha << *arg_h << "\n";
  std::cout << "v == " << std::boolalpha << *arg_v << "\n";
  std::cout << "(x, y) == (";
  std::cout << (arg_x ? *arg_x : -1) << ", " << (arg_y ? *arg_y : -1);
  std::cout << ")\n";


  return 0;
}
