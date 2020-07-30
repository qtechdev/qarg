#ifndef __QARG_TPP__
#define __QARG_TPP__
#include <sstream>
#include <algorithm>
#include <cctype>

namespace qarg {
  template <typename T> type_hint tohint() { return type_hint::NONE; }
  template <> type_hint tohint<bool>() { return type_hint::BOOL; }
  template <> type_hint tohint<int>() { return type_hint::INT; }
  template <> type_hint tohint<double>() { return type_hint::FLOAT; }
  template <> type_hint tohint<std::string>() { return type_hint::STRING; }


  template <typename T>
  void parser::add(
    const char c, const bool r, const std::string d
  ) {
    spec[c] = {r, d, tohint<T>()};
  }

  // convert option to type T
  template <typename T>
  std::optional<T> parser::get(const char c) const {
    auto s = (*this)(c);
    if (!s) {
      return {};
    }

    T v;
    std::stringstream ss(*s);

    ss >> v;

    return {v};
  }

  // specialisation for bool
  template <>
  std::optional<bool> parser::get<bool>(const char c) const {
    auto s = (*this)(c);
    if (!s) {
      return {false};
    }

    std::string sl = *s;
    std::transform(
      sl.begin(), sl.end(), sl.begin(),
      [](unsigned char c){ return std::tolower(c); }
    );

    for (const auto &sv : falsey) {
      if (sl == sv) {
        return {false};
      }
    }

    return {true};
  }

  // specialisation for string
  template <>
  std::optional<std::string> parser::get<std::string>(const char c)  const {
    auto s = (*this)(c);
    if (!s) {
      return {};
    }

    return *s;
  }


}

#endif // __QARG_TPP__
