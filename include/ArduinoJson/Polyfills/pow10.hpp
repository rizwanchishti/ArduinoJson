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

/* Fast and small but error is too big :-(
template <typename T>
inline T pow10(T x, int exponent) {
  T factor;
  if (exponent < 0) {
    factor = 0.1;
    exponent = -exponent;
  } else {
    factor = 10;
  }

  while (exponent != 0) {
    if (exponent % 2) {
      value *= factor;
      exponent--;
    }
    factor *= factor;
    exponent /= 2;
  }

  return value;
}
*/

template <typename T>
inline T pow10(T value, int exponent) {
  while (exponent != 0) {
    if (exponent >= 4) {
      value *= 1e4;
      exponent -= 4;
    } else if (exponent >= 1) {
      value *= 1e1;
      exponent -= 1;
    } else if (exponent <= -1) {
      value /= 1e1;
      exponent += 1;
    } else if (exponent <= -4) {
      value /= 1e4;
      exponent += 4;
    }
  }

  return value;
}

#else  // optimize for speed

// If Visual Studo <= 2010
#if defined(_MSC_VER) && _MSC_VER <= 1600

template <typename T>
T pow10(T mantissa, int exponent) {
  return mantissa * ::pow(static_cast<T>(10), static_cast<T>(exponent));
}

#else

template <typename T>
T pow10(T mantissa, int exponent) {
  return mantissa * ::pow(10, exponent);
}

#endif

#endif
}
}
