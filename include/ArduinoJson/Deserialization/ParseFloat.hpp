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
  T sign = +1;
  if (*s == '-') {
    sign = -1;
    s++;
  } else if (*s == '+') {
    s++;
  }

  // 2. integer part
  T result = 0.0;
  while ('0' <= *s && *s <= '9') {
    result = result * 10 + (*s - '0');
    s++;
  }

  // 3. decimal part
  if (*s == '.') {
    s++;
    T factor = 0.1;
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
}
}