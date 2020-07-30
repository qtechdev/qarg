#ifndef __QARG_HPP__
#define __QARG_HPP__

#include <map>
#include <string>
#include <vector>
#include <optional>

namespace qarg {
  enum class type_hint {
    NONE    = 0,
    BOOL    = 1,
    INT     = 2,
    FLOAT   = 4,
    STRING  = 8
  };

  struct option {
    bool requires_arg;
    std::string description;
    type_hint hint;
  };

  std::string hinttostr(const type_hint t);

  class parser {
  public:
    void add(
      const char c, const bool requires_arg, const std::string description=""
    );

    template <typename T>
    void add(
      const char c, const bool requires_arg, const std::string description=""
    );

    void parse(int argc, const char *argv[]);

    std::optional<std::string> operator()(const char c) const;
    std::string help() const;
  private:
    std::map<char, option> spec;
    std::map<char, std::optional<std::string>> options;
    std::vector<std::string> positional;
    std::optional<char> current_option;
    bool is_parsing_options = true;

    void check(const char *arg);
    void flush_option();
    void push_back_arg(const char *arg);
  };
}

#include "qarg.tpp"

#endif // __QARG_HPP__
