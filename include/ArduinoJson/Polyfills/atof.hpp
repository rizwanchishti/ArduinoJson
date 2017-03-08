// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../TypeTraits/IntegerTypes.hpp"

namespace ArduinoJson {
namespace Polyfills {

#if ARDUINOJSON_REPLACE_ATOF

template <typename TResult, typename TMantissa, typename TExponent>
TResult make_float(TMantissa mantissa, TExponent exponent) {
  TResult table[] = {1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256};
  TResult result = static_cast<TResult>(mantissa);

  if (exponent >= 0) {
    for (uint8_t i = 0; exponent > 0 && i < 9; i++) {
      if (exponent & 1) result *= table[i];
      exponent = static_cast<TExponent>(exponent >> 1);
    }
  } else {
    exponent = static_cast<TExponent>(-exponent);
    for (uint8_t i = 0; exponent > 0 && i < 9; i++) {
      if (exponent & 1) result /= table[i];
      exponent = static_cast<TExponent>(exponent >> 1);
    }
  }

  return result;
}

template <typename T>
inline T parseFloat(const char* s) {
  typedef typename TypeTraits::sint<sizeof(T)>::type mantissa_t;
  typedef typename TypeTraits::sint<sizeof(T) / 4>::type exponent_t;

  bool negative_result = false;
  switch (*s) {
    case '-':
      negative_result = true;
    case '+':
      s++;
  }

  if (!strcmp(s, "NaN")) return Polyfills::nan<T>();

  mantissa_t mantissa = 0;
  exponent_t exponent = 0;

  while ('0' <= *s && *s <= '9') {
    if (mantissa < 1e12)
      mantissa = mantissa * 10UL + (*s - '0');
    else
      exponent++;
    s++;
  }

  if (*s == '.') {
    s++;
    while ('0' <= *s && *s <= '9') {
      mantissa = mantissa * 10 + (*s - '0');
      s++;
      exponent--;
    }
  }

  if (negative_result) mantissa = -mantissa;

  if (*s == 'e' || *s == 'E') {
    s++;
    exponent_t accumulator = 0;
    bool negative = false;
    if (*s == '-') {
      negative = true;
      s++;
    } else if (*s == '+') {
      s++;
    }

    while ('0' <= *s && *s <= '9') {
      accumulator = static_cast<exponent_t>(accumulator * 10 + (*s - '0'));
      s++;
    }

    if (negative) {
      exponent = static_cast<exponent_t>(exponent - accumulator);
    } else {
      exponent = static_cast<exponent_t>(exponent + accumulator);
    }
  }

  return make_float<T>(mantissa, exponent);
}

inline bool isdigit(char c) {
  return '0' <= c && c <= '9';
}

inline bool isFloat(const char* s) {
  if (*s == '-' || *s == '+') s++;
  if (!strcmp(s, "NaN")) return true;

  while (isdigit(*s)) s++;

  bool has_dot = *s == '.';

  if (has_dot) {
    s++;
    while (isdigit(*s)) s++;
  }

  bool has_exponent = *s == 'e' || *s == 'E';

  if (has_exponent) {
    s++;
    if (*s == '-' || *s == '+') s++;
    while (isdigit(*s)) s++;
  }

  return (has_dot || has_exponent) && *s == '\0';
}

#else

inline bool isFloat(const char *input) {
  char *end;
  errno = 0;
  strtod(input, &end);
  return *end == '\0' && errno == 0;
}

template <typename T>
inline T parseFloat(const char *input) {
  char *end;
  return strtod(input, &end);
}

#endif
}
}
