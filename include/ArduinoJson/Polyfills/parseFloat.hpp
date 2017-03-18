// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include "../TypeTraits/FloatTraits.hpp"
#include "./ctype.hpp"
#include "./math.hpp"

namespace ArduinoJson {
namespace Polyfills {

template <typename TResult, typename TMantissa, typename TExponent>
TResult make_float(TMantissa mantissa, TExponent exponent) {
  typedef TypeTraits::FloatTraits<TResult> traits;
  TResult result = static_cast<TResult>(mantissa);

  return result * traits::pow10(exponent);
}

template <typename T>
inline T parseFloat(const char* s) {
  typedef TypeTraits::FloatTraits<T> traits;
  typedef typename traits::mantissa_type mantissa_t;
  typedef typename traits::exponent_type exponent_t;

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
  exponent_t exponent_offset = 0;

  while (isdigit(*s)) {
    if (mantissa < traits::mantissa_max / 10)
      mantissa = mantissa * 10 + (*s - '0');
    else
      exponent_offset++;
    s++;
  }

  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (mantissa < traits::mantissa_max / 10) {
        mantissa = mantissa * 10 + (*s - '0');
        exponent_offset--;
      }
      s++;
    }
  }

  int exponent = exponent_offset;
  if (*s == 'e' || *s == 'E') {
    s++;
    int accumulator = 0;
    int sign = +1;
    if (*s == '-') {
      sign = -1;
      s++;
    } else if (*s == '+') {
      s++;
    }

    while (isdigit(*s)) {
      accumulator = accumulator * 10 + sign * (*s - '0');
      if (accumulator + exponent_offset > traits::exponent_max)
        return negative_result ? -Polyfills::inf<T>() : Polyfills::inf<T>();
      if (accumulator - exponent_offset < -traits::exponent_max)
        return negative_result ? -0 : 0;
      s++;
    }
    exponent = exponent_offset + accumulator;
  }

  T result = make_float<T>(mantissa, exponent);

  // For some reason, negating the result instead of the mantissa leads to much
  // smaller code (74 bytes on AVR)
  return negative_result ? -result : result;
}
}
}
