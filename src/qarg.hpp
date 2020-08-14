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
  std::string hinttostr(const type_hint t);

  // false values for bool conversion
  // note: argument is converted to lowercase before comparison
  constexpr std::array<const std::string_view, 3> falsey = {
    "false", "f", "0"
  };

  struct option {
    std::string description;
    type_hint hint;
    bool requires_arg;
    bool is_required;
  };

  class parser {
  public:
    // adds an option to the parser spec (T is a hint only used by help())
    template <typename T>
    void add(
      const char c, const std::string description="",
      const bool is_required=false
    );

    // used to specify config file path
    void config(
      const char c, const std::string description="",
      const bool is_required=false
    );

    // parses args according to the spec
    // throws std::invalid_argument if provided args do not match the spec
    void parse(int argc, const char *argv[]);

    // returns argument value without any processing
    std::optional<std::string> operator()(const char c) const;
    // converts argument value to type T
    template <typename T>
    std::optional<T> get(const char c) const;

    // parses the spec and returns a formatted string
    std::string help() const;
  private:
    std::map<char, option> spec;
    std::map<char, std::optional<std::string>> options;
    std::vector<std::string> positional;
    std::optional<char> current_option;
    bool is_parsing_options = true;
    bool has_config = false;
    char config_opt = 'c';

    void check(const char *arg);
    void flush_option();
    void push_back_arg(const char *arg);
  };
}

#include "qarg.tpp"

#endif // __QARG_HPP__
