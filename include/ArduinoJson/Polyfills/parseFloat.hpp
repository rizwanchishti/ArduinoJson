// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../TypeTraits/IntegerTypes.hpp"
#include "./ctype.hpp"
#include "./math.hpp"

namespace ArduinoJson {
namespace Polyfills {

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

template <typename TResult, typename TMantissa, typename TExponent>
TResult make_float(TMantissa mantissa, TExponent exponent) {
  TResult result = static_cast<TResult>(mantissa);

  if (exponent >= 0) {
    for (uint8_t i = 0; exponent > 0 && i < 9; i++) {
      if (exponent & 1)
        result *= FloatTraits<TResult>::binary_exponentiation(i);
      exponent = static_cast<TExponent>(exponent >> 1);
    }
  } else {
    exponent = static_cast<TExponent>(-exponent);
    for (uint8_t i = 0; exponent > 0 && i < 9; i++) {
      if (exponent & 1)
        result /= FloatTraits<TResult>::binary_exponentiation(i);
      exponent = static_cast<TExponent>(exponent >> 1);
    }
  }

  return result;
}

template <typename T>
inline T parseFloat(const char* s) {
  typedef typename FloatTraits<T>::mantissa_type mantissa_t;
  typedef typename FloatTraits<T>::exponent_type exponent_t;

  bool negative_result = false;
  switch (*s) {
    case '-':
      negative_result = true;
    case '+':
      s++;
  }

  if (*s == 'n' || *s == 'N') return Polyfills::nan<T>();
  if (*s == 'i' || *s == 'I')
    return negative_result ? -Polyfills::inf<T>() : Polyfills::inf<T>();

  mantissa_t mantissa = 0;
  exponent_t exponent = 0;

  while (isdigit(*s)) {
    if (mantissa < FloatTraits<T>::mantissa_max / 10)
      mantissa = mantissa * 10 + (*s - '0');
    else
      exponent++;
    s++;
  }

  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (mantissa < FloatTraits<T>::mantissa_max / 10) {
        mantissa = mantissa * 10 + (*s - '0');
        exponent--;
      }
      s++;
    }
  }

  if (negative_result) mantissa = -mantissa;

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

    while (isdigit(*s)) {
      accumulator = static_cast<exponent_t>(accumulator * 10 + (*s - '0'));
      s++;
    }

    if (negative) {
      exponent = static_cast<exponent_t>(exponent - accumulator);
    } else {
      exponent = static_cast<exponent_t>(exponent + accumulator);
    }
  }

  return make_float<T>(mantissa, exponent);
}
}
}
