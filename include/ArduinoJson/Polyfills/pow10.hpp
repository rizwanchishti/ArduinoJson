// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace Polyfills {

#ifdef ARDUINO

// on embedded platform, favor code size over speed

template <typename T>
inline T pow10(int n) {
  T result = 1;
  while (n > 0) {
    result *= 10;
    n--;
  }
  return result;
}

#else

// on non-embedded platform, favor speed over code size

template <typename T>
inline T pow10(int n) {
  return ::pow(10, n);
}

#endif
}
}
