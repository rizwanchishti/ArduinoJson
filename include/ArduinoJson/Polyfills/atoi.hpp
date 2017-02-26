// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

#include <stdlib.h>

namespace ArduinoJson {
namespace Polyfills {
template <typename T>
T atoi(const char *s);

template <>
inline long atoi<long>(const char *s) {
  return ::strtol(s, NULL, 10);
}

template <>
inline unsigned long atoi<unsigned long>(const char *s) {
  return ::strtoul(s, NULL, 10);
}

template <>
inline int atoi<int>(const char *s) {
  return ::atoi(s);
}

#if ARDUINOJSON_USE_LONG_LONG
template <>
inline long long atoi<long long>(const char *s) {
  return ::strtoll(s, NULL, 10);
}

template <>
inline unsigned long long atoi<unsigned long long>(const char *s) {
  return ::strtoull(s, NULL, 10);
}
#endif

#if ARDUINOJSON_USE_INT64
template <>
inline __int64 atoi<__int64>(const char *s) {
  return ::_strtoi64(s, NULL, 10);
}

template <>
inline unsigned __int64 atoi<unsigned __int64>(const char *s) {
  return ::_strtoui64(s, NULL, 10);
}
#endif
}
}
