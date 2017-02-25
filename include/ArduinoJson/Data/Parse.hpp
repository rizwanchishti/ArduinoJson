// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <stdlib.h>
#include "../Polyfills/pow10.hpp"

namespace ArduinoJson {
namespace Internals {
template <typename TFloat>
TFloat parse(const char *);

template <>
inline double parse<double>(const char *s) {
  // 1. sign
  double sign = +1;
  if (*s == '-') {
    sign = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  // 2. integer part
  double result = 0.0;
  while ('0' <= *s && *s <= '9') {
    result = result * 10 + (*s - '0');
    s++;
  }

  // 3. decimal part
  if (*s == '.') {
    s++;
    double factor = 0.1;
    while ('0' <= *s && *s <= '9') {
      result += factor * (*s - '0');
      factor /= 10;
      s++;
    }
  }

  // 4. exponentation
  if (*s == 'e' || *s == 'E') {
    s++;

    bool negative_exponent = false;
    if (*s == '-') {
      negative_exponent = true;
      s++;
    } else if (*s == '+') {
      s++;
    }

    int exponent = 0;
    while ('0' <= *s && *s <= '9') {
      exponent = exponent * 10 + (*s - '0');
      s++;
    }

    if (negative_exponent)
      result /= Polyfills::pow10(exponent);
    else
      result *= Polyfills::pow10(exponent);
  }

  return sign * result;
}

template <>
inline float parse<float>(const char *s) {
  return static_cast<float>(parse<double>(s));
}

template <>
inline long parse<long>(const char *s) {
  return strtol(s, NULL, 10);
}

template <>
inline unsigned long parse<unsigned long>(const char *s) {
  return strtoul(s, NULL, 10);
}

template <>
inline int parse<int>(const char *s) {
  return atoi(s);
}

#if ARDUINOJSON_USE_LONG_LONG
template <>
inline long long parse<long long>(const char *s) {
  return strtoll(s, NULL, 10);
}

template <>
inline unsigned long long parse<unsigned long long>(const char *s) {
  return strtoull(s, NULL, 10);
}
#endif

#if ARDUINOJSON_USE_INT64
template <>
inline __int64 parse<__int64>(const char *s) {
  return _strtoi64(s, NULL, 10);
}

template <>
inline unsigned __int64 parse<unsigned __int64>(const char *s) {
  return _strtoui64(s, NULL, 10);
}
#endif
}
}
