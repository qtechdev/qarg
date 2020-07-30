#ifndef __QARG_TPP__
#define __QARG_TPP__

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
}

#endif // __QARG_TPP__
