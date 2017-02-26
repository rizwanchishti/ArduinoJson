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
inline T pow10(T value, int n) {
  while (n < 0) {
    value /= 10;
    n++;
  }
  while (n > 0) {
    value *= 10;
    n--;
  }
  return value;
}

#else  // optimize for speed

template <typename T>
T pow10(T value, int n) {
  return value * ::pow(10, n);
}

// If Visual Studo <= 2010
#if defined(_MSC_VER) && _MSC_VER <= 1600

template <>
float pow10<float>(float value, int n) {
  return value * ::powf(10.0f, n);
}

#endif

#endif
}
}
