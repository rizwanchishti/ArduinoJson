// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../Configuration.hpp"

namespace ArduinoJson {
namespace Polyfills {

#if ARDUINOJSON_OPTIMIZE_SIZE

template <typename T>
inline T pow10(int n) {
  T result = 1;
  while (n < 0) {
    result /= 10;
    n++;
  }
  while (n > 0) {
    result *= 10;
    n--;
  }
  return result;
}

#else  // optimize for speed

template <typename T>
T pow10(int n) {
  return ::pow(10, n);
}

// If Visual Studo <= 2010
#if defined(_MSC_VER) && _MSC_VER <= 1600

template <>
float pow10<float>(int n) {
  return ::powf(10.0f, n);
}

#endif

#endif
}
}
