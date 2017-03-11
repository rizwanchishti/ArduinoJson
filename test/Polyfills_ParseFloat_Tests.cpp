// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <gtest/gtest.h>
#include <ArduinoJson/Polyfills/parseFloat.hpp>

using namespace ArduinoJson::Polyfills;

struct Polyfills_ParseFloat_Float_Tests : testing::Test {
  void check(const char* input, float expected) {
    float actual = parseFloat<float>(input);
    EXPECT_FLOAT_EQ(expected, actual) << input;
  }

  void checkNaN(const char* input) {
    float result = parseFloat<float>(input);
    EXPECT_TRUE(result != result) << input;
  }

  void checkInf(const char* input, bool negative) {
    float x = parseFloat<float>(input);
    if (negative)
      EXPECT_TRUE(x < 0) << input;
    else
      EXPECT_TRUE(x > 0) << input;
    EXPECT_TRUE(x == x && x * 2 == x) << input;
  }
};
#define TEST_FLOAT(X) TEST_F(Polyfills_ParseFloat_Float_Tests, X)

struct Polyfills_ParseFloat_Double_Tests : testing::Test {
  void check(const char* input, double expected) {
    double actual = parseFloat<double>(input);
    EXPECT_DOUBLE_EQ(expected, actual) << input;
  }

  void checkNaN(const char* input) {
    double result = parseFloat<double>(input);
    EXPECT_TRUE(result != result) << input;
  }
};
#define TEST_DOUBLE(X) TEST_F(Polyfills_ParseFloat_Double_Tests, X)

TEST_DOUBLE(Short_NoExponent) {
  check("3.14", 3.14);
  check("-3.14", -3.14);
  check("+3.14", +3.14);
}

TEST_FLOAT(Float_Short_NoExponent) {
  check("3.14", 3.14f);
  check("-3.14", -3.14f);
  check("+3.14", +3.14f);
}

TEST_DOUBLE(Short_NoDot) {
  check("1E+308", 1E+308);
  check("-1E+308", -1E+308);
  check("+1E-308", +1E-308);
  check("+1e+308", +1e+308);
  check("-1e-308", -1e-308);
}

TEST_FLOAT(Short_NoDot) {
  check("1E+38", 1E+38f);
  check("-1E+38", -1E+38f);
  check("+1E-38", +1E-38f);
  check("+1e+38", +1e+38f);
  check("-1e-38", -1e-38f);
}

TEST_DOUBLE(VeryLong) {
  check("0.00000000000000000000000000000001", 1e-32);
  check("100000000000000000000000000000000.0", 1e+32);
  check("100000000000000000000000000000000.00000000000000000000000000000",
        1e+32);
}

TEST_FLOAT(VeryLong) {
  check("0.00000000000000000000000000000001", 1e-32f);
  check("100000000000000000000000000000000.0", 1e+32f);
  check("100000000000000000000000000000000.00000000000000000000000000000",
        1e+32f);
}

TEST_DOUBLE(NaN) {
  checkNaN("NaN");
  checkNaN("nan");
}

TEST_FLOAT(NaN) {
  checkNaN("NaN");
  checkNaN("nan");
}

TEST_FLOAT(Infinity) {
  checkInf("Infinity", false);
  checkInf("+Infinity", false);
  checkInf("-Infinity", true);
  checkInf("inf", false);
  checkInf("+inf", false);
  checkInf("-inf", true);
}
