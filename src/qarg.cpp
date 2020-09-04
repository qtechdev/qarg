#include <cstdio>     // std::snprintf
#include <cstring>    // std::strlen
#include <fstream>    // std::ifstream
#include <stdexcept>  // std::invalid_argument

#include <nlohmann/json.hpp>

#include "qarg.hpp"

std::string qarg::hinttostr(const type_hint t) {
  switch (t) {
    case type_hint::BOOL: return "<bool>";
    case type_hint::INT: return "<int>";
    case type_hint::FLOAT: return "<float>";
    case type_hint::STRING: return "<string>";
    default: return "<unknown>";
  }
}

void qarg::parser::config(
  const char c, const std::string d, const bool r
) {
  auto h = type_hint::STRING;

  spec[c].description = d;
  spec[c].hint = h;
  spec[c].requires_arg = true;
  spec[c].is_required = r;

  has_config = true;
  config_opt = c;
}

void qarg::parser::parse(int argc, const char *argv[]) {
  for (int i = 1; i < argc; ++i) {
    check(argv[i]);
  }

  flush_option();

  if (spec.find('h') != spec.end() && *get<bool>('h')) {
    return;
  }

  if (has_config) {
    auto config_path = get<std::string>(config_opt);
    if (config_path) {
      std::ifstream ifs(*config_path);

      nlohmann::json j;
      ifs >> j;

      for (const auto &[k, v] : j.items()) {
        std::stringstream kss;
        kss << '-' << k;
        check(kss.str().c_str());

        if (v.is_string()) {
          check(std::string(v).c_str());
        } else {
          std::stringstream vss;
          vss << v;
          check(vss.str().c_str());
        }
      }
    }
  }

  for (auto &[k, v] : spec) {
    if (!v.is_required) { continue; }

    if (options.find(k) == options.end()) {
      char buf[256];
      std::snprintf(buf, 256, "-%c is a required argument", k);
      throw std::invalid_argument(buf);
    }
  }
}

std::optional<std::string> qarg::parser::operator()(const char c) const {
  if (spec.find(c) == spec.end()) {
    char buf[256];
    std::snprintf(buf, 256, "-%c is not a recognised argument", c);
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
    std::snprintf(
      buf, 256,
      "-%c %-10s %s",
      k,
      (spec.at(k).requires_arg ? hinttostr(v.hint) : "" ).c_str(),
      v.description.c_str()
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
      std::snprintf(buf, 256, "-%c is not a recognised argument", c);
      throw std::invalid_argument(buf);
    }

    if (!spec.at(c).requires_arg) {
      options[c] = "true";
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
    std::snprintf(buf, 256, "-%c requires an argument", *current_option);
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
