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

  if (exponent >= 0) {
    for (uint8_t i = 0; exponent > 0 && i < 9; i++) {
      if (exponent & 1) result *= traits::binary_exponentiation(i);
      exponent = static_cast<TExponent>(exponent >> 1);
    }
  } else {
    exponent = static_cast<TExponent>(-exponent);
    for (uint8_t i = 0; exponent > 0 && i < 9; i++) {
      if (exponent & 1) result /= traits::binary_exponentiation(i);
      exponent = static_cast<TExponent>(exponent >> 1);
    }
  }

  return result;
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
  exponent_t exponent = 0;

  while (isdigit(*s)) {
    if (mantissa < traits::mantissa_max / 10)
      mantissa = mantissa * 10 + (*s - '0');
    else
      exponent++;
    s++;
  }

  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (mantissa < traits::mantissa_max / 10) {
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
