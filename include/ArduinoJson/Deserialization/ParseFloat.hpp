// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Polyfills/pow10.hpp"
#include "../TypeTraits/IntegerTypes.hpp"

namespace ArduinoJson {
namespace Internals {
template <typename T>
inline T parseFloat(const char *s) {
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

  T result =
      Polyfills::pow10(static_cast<T>(mantissa), exponent - decimalsPlaces);

  return negative_result ? -result : result;
}
}
}
