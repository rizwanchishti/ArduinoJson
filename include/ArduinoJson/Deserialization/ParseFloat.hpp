// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Polyfills/pow10.hpp"

namespace ArduinoJson {
namespace Internals {
template <typename T>
inline T parseFloat(const char *s) {
  // 1. sign
  bool negative_result = false;
  if (*s == '-') {
    negative_result = true;
    s++;
  } else if (*s == '+') {
    s++;
  }

  uint64_t mantissa = 0;
  while ('0' <= *s && *s <= '9') {
    mantissa = mantissa * 10UL + (*s - '0');
    s++;
  }

  int decimalsPlaces = 0;
  if (*s == '.') {
    s++;
    while ('0' <= *s && *s <= '9') {
      mantissa = mantissa * 10 + (*s - '0');
      s++;
      decimalsPlaces++;
    }
  }

  int exponent = 0;
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
      exponent = exponent * 10 + (*s - '0');
      s++;
    }

    if (negative_exponent) exponent = -exponent;
  }

  T result =
      static_cast<T>(mantissa) * Polyfills::pow10<T>(exponent - decimalsPlaces);

  return negative_result ? -result : result;
}
}
}
