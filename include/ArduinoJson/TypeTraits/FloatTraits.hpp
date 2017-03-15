// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <stdint.h>

namespace ArduinoJson {
namespace TypeTraits {

template <typename T, size_t = sizeof(T)>
struct FloatTraits {};

#ifndef ARDUINO_ARCH_AVR  // double is 32 bits, so 1e64 gives a warning
template <typename T>
struct FloatTraits<T, 8 /*64bits*/> {
  typedef int64_t mantissa_type;
  static const short mantissa_bits = 52;
  static const mantissa_type mantissa_max =
      (static_cast<mantissa_type>(1) << mantissa_bits) - 1;

  typedef int16_t exponent_type;

  static T binary_exponentiation(uint8_t i) {
    T table[] = {1e1, 1e2, 1e4, 1e8, 1e16, 1e32, 1e64, 1e128, 1e256};
    return table[i];
  }
};
#endif

template <typename T>
struct FloatTraits<T, 4 /*32bits*/> {
  typedef int32_t mantissa_type;
  static const short mantissa_bits = 23;
  static const mantissa_type mantissa_max =
      (static_cast<mantissa_type>(1) << mantissa_bits) - 1;

  typedef int8_t exponent_type;

  static T binary_exponentiation(uint8_t i) {
    T table[] = {1e1f, 1e2f, 1e4f, 1e8f, 1e16f, 1e32f};
    return table[i];
  }
};
}
}
