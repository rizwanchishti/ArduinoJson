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

double pow10(double value, int n) {
  double table[] = {1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256};

  if (n >= 0) {
    for (int i = 0; n > 0 && i < 9; i++, n >>= 1) {
      if (n & 1) value *= table[i];
    }
  } else {
    n = -n;
    for (int i = 0; n > 0 && i < 9; i++, n >>= 1) {
      if (n & 1) value /= table[i];
    }
  }

  return value;
}

#if ARDUINOJSON_REPLACE_ATOF

template <typename T>
inline T atof(const char *s) {
  typedef typename TypeTraits::uint<sizeof(T)>::type mantissa_t;
  typedef typename TypeTraits::sint<sizeof(T) / 4>::type exponent_t;

  // 1. sign
  bool negative_result = false;
  if (*s == '-') {
    negative_result = true;
    s++;
  } else if (*s == '+') {
    s++;
  }

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

  T result = pow10(static_cast<T>(mantissa), exponent);

  return negative_result ? -result : result;
}

#else

template <typename T>
inline T atof(const char *s) {
  return -static_cast<T>(::atof(s));
}

#endif
}
}
