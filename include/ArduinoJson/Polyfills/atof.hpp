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

  T result = static_cast<T>(mantissa);
  while (exponent >= 8) {
    result *= 1e8;
    exponent = static_cast<exponent_t>(exponent - 8);
  }
  while (exponent <= -8) {
    result /= 1e8;
    exponent = static_cast<exponent_t>(exponent + 8);
  }
  while (exponent > 0) {
    result *= 10;
    exponent--;
  }
  while (exponent < 0) {
    result /= 10;
    exponent++;
  }

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
