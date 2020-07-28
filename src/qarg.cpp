#include <cstdio>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include "qarg.hpp"

void qarg::parser::add(const char c, const bool r, const std::string d) {
  spec[c] = {r, d};
}


void qarg::parser::parse(int argc, const char *argv[]) {
  for (int i = 1; i < argc; ++i) {

    check(argv[i]);

  }

  flush_option();
}

std::optional<std::string> qarg::parser::operator()(const char c) const {
  if (spec.find(c) == spec.end()) {
    char buf[256];
    snprintf(buf, 256, "-%c is not a recognised argument", c);
    throw std::invalid_argument(buf);
  }

  auto f = options.find(c);
  if (f == options.end()) {
    return {};
  }

  return options.at(c);
}

std::string qarg::parser::help() const {
  std::stringstream ss;

  ss << "options:\n";

  for (auto &[k,v] : spec) {
    char buf[256];
    snprintf(
      buf, 256,
      "-%c %-10s %s",
      k, (v.requires_arg ? "<value>" : ""), v.description.c_str()
    );
    ss << buf << "\n";
  }

  return ss.str();
}

void qarg::parser::check(const char *arg) {
  int s = std::strlen(arg);

  // single character. _a single hyphen is special, but not yet implemented_
  if (s < 2) {
    push_back_arg(arg); return;
  }

  // arg is not an option
  if (arg[0] != '-') {
    push_back_arg(arg); return;
  }

  // option parsing is terminated by a double hyphen
  if (!is_parsing_options) {
    push_back_arg(arg); return;
  }

  flush_option();
  if (std::strcmp(arg, "--") == 0) {
    is_parsing_options = false; return;
  }

  // for each option in group
  for (int i = 1; i < s; ++i) {
    if (current_option) {
      flush_option();
    }

    char c = arg[i];
    if (spec.find(c) == spec.end()) {
      char buf[256];
      snprintf(buf, 256, "-%c is not a recognised argument", c);
      throw std::invalid_argument(buf);
    }

    if (!spec.at(c).requires_arg) {
      options[c] = "";
    } else {
      current_option = c;
    }
  }
}

void qarg::parser::flush_option() {
  if (!current_option) {
    return;
  }

  if (spec[*current_option].requires_arg) {
    char buf[256];
    snprintf(buf, 256, "-%c requires an argument", *current_option);
    throw std::invalid_argument(buf);
  }

  options[*current_option] = "";
}

void qarg::parser::push_back_arg(const char *arg) {
  if (current_option) {
    options[*current_option] = arg;
    current_option = {};
  } else {
    positional.push_back(arg);
  }
}
