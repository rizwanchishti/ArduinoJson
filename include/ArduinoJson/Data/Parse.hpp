// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <stdlib.h>

namespace ArduinoJson {
namespace Internals {
template <typename TFloat>
TFloat parse(const char *);

template <>
inline double parse<double>(const char *s) {
  double result = 0.0;
  double factor = 1.0;
  bool exp_factor = 1;  // default 1 - positive exponent. Use 0 for negative
  double exponent = 0;
  double multiplier = 1;  // multiplier for exponent
  bool decimals = false;
  char c;

  if (*s == '\0') return NAN;

  if (*s == '-') {
    factor = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  while ((c = *s)) {
    if (c == '.') {
      decimals = true;
      s++;
      continue;
    }

    if (c == 'e' || c == 'E') {
      s++;
      if (*s == '-') {
        exp_factor = 0;
        s++;
      } else if (*s == '+') {
        s++;
      }
      while ((c = *s) != '\0') {  // this while loop for the exponent
        int d = c - '0';
        if (d < 0 || d > 9) {
          break;
        }
        exponent = exponent * 10 + d;
        s++;
      }
    }

    else {
      int d = c - '0';
      if (d < 0 || d > 9) {
        break;
      }

      result = 10.0 * result + d;
      if (decimals) {
        factor *= 0.1;
      }

      s++;
    }
  }
  result *= factor;

  if (exponent) {
    multiplier = pow(10, exponent);
    if (exp_factor)
      result *= multiplier;
    else
      result /= multiplier;
  }

  return result;
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
