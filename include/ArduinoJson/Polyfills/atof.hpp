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

template <typename T>
inline T atof(const char *s) {
  return static_cast<T>(::atof(s));
}

#else

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
  while ('0' <= *s && *s <= '9') {
    mantissa = mantissa * 10UL + (*s - '0');
    s++;
  }

  exponent_t decimalsPlaces = 0;
  if (*s == '.') {
    s++;
    while ('0' <= *s && *s <= '9') {
      mantissa = mantissa * 10 + (*s - '0');
      s++;
      decimalsPlaces++;
    }
  }

  exponent_t exponent = 0;
  if (*s == 'e' || *s == 'E') {
    s++;
    bool negative_exponent = false;
    if (*s == '-') {
      negative_exponent = true;
      s++;
    } else if (*s == '+') {
      s++;
    }

    while ('0' <= *s && *s <= '9') {
      exponent = static_cast<exponent_t>(exponent * 10 + (*s - '0'));
      s++;
    }

    if (negative_exponent) exponent = static_cast<exponent_t>(-exponent);
  }
  exponent = static_cast<exponent_t>(exponent - decimalsPlaces);

  T result = static_cast<T>(mantissa);
  while (exponent >= 4) {
    result *= 1e4;
    exponent = static_cast<exponent_t>(exponent - 4);
  }
  while (exponent > 0) {
    result *= 10;
    exponent--;
  }
  while (exponent < 0) {
    result /= 10;
    exponent++;
  }
  while (exponent <= -4) {
    result /= 1e4;
    exponent = static_cast<exponent_t>(exponent + 4);
  }

  return negative_result ? -result : result;
}

#endif
}
}
