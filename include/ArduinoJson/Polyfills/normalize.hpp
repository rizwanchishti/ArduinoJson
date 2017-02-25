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
short normalize(T& value) {
  short powersOf10 = 0;
  while (value && value < 1) {
    powersOf10--;
    value *= 10;
  }
  while (value > 10) {
    powersOf10++;
    value /= 10;
  }
  return powersOf10;
}

#else  // optimize for speed

template <typename T>
short normalize(T& value) {
  if (value == 0.0) return 0;

  short powersOf10 = static_cast<short>(floor(log10(value)));
  value /= pow(T(10), powersOf10);

  return powersOf10;
}

#endif
}
}
