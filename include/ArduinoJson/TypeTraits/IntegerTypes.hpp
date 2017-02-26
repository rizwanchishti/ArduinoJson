// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#pragma once

namespace ArduinoJson {
namespace TypeTraits {

template <size_t Size>
struct uint {};

template <>
struct uint<4> {
  typedef uint32_t type;
};

template <>
struct uint<8> {
  typedef uint64_t type;
};
}
}
