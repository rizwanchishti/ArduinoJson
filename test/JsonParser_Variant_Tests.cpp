// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <gtest/gtest.h>

class JsonParser_Variant_Tests : public testing::Test {
 protected:
  void whenInputIs(const char* jsonString) {
    strcpy(_jsonString, jsonString);
    _result = _jsonBuffer.parse(_jsonString);
  }

  template <typename T>
  void resultMustEqual(T expected) {
    EXPECT_EQ(expected, _result.as<T>());
  }

  void resultMustEqual(const char* expected) {
    EXPECT_STREQ(expected, _result.as<char*>());
  }

  void resultMustEqual(float expected) {
    float actual = _result.as<float>();
    if (expected != expected)
      EXPECT_TRUE(actual != actual);
    else
      EXPECT_FLOAT_EQ(expected, actual);
  }

  void resultMustEqual(double expected) {
    double actual = _result.as<double>();
    if (expected != expected)
      EXPECT_TRUE(actual != actual);
    else
      EXPECT_DOUBLE_EQ(expected, actual);
  }

  template <typename T>
  void resultTypeMustBe() {
    EXPECT_TRUE(_result.is<T>()) << _jsonString;
  }

  void resultMustBeInvalid() {
    EXPECT_FALSE(_result.success());
  }
  void resultMustBeValid() {
    EXPECT_TRUE(_result.success());
  }

  template <typename T>
  void verify(const char* input, T expected) {
    whenInputIs(input);
    resultMustBeValid();
    resultTypeMustBe<T>();
    resultMustEqual(expected);
  }

 private:
  DynamicJsonBuffer _jsonBuffer;
  JsonVariant _result;
  char _jsonString[256];
};

TEST_F(JsonParser_Variant_Tests, EmptyObject) {
  whenInputIs("{}");
  resultMustBeValid();
  resultTypeMustBe<JsonObject>();
}

TEST_F(JsonParser_Variant_Tests, EmptyArray) {
  whenInputIs("[]");
  resultMustBeValid();
  resultTypeMustBe<JsonArray>();
}

TEST_F(JsonParser_Variant_Tests, Integer) {
  verify("42", 42);
  verify("-42", -42);
}

TEST_F(JsonParser_Variant_Tests, Double) {
  verify<double>("3.14", 3.14);
  verify<double>("-3.14", -3.14);
  verify<double>("+3.14", +3.14);
  verify<double>("1E+10", 1E+10);
  verify<double>("-1E+10", -1E+10);
  verify<double>("1.234E10", 1.234E10);
  verify<double>("1.234E+10", 1.234E+10);
  verify<double>("1.79769e308", 1.79769e308);
  verify<double>("+1.79769e+308", +1.79769e+308);
  verify<double>("-1.79769e-308", -1.79769e-308);
  verify<double>("1.7976931348623157", 1.7976931348623157);
  verify<double>("1.7976931348623157e308", 1.7976931348623157e308);
  verify<double>("1.7976931348623157e+308", 1.7976931348623157e+308);
  verify<double>("0.017976931348623157e+310", 0.017976931348623157e+310);
  verify<double>("0.00000000000000000000000000000001", 1e-32);
  verify<double>("100000000000000000000000000000000.0", 1e+32);
  verify<double>("NaN", NAN);
  /* verify<double>("Infinity", INF);
    verify<double>("+Infinity", INF);
    verify<double>("-Infinity", -INF);*/
}

TEST_F(JsonParser_Variant_Tests, Float) {
  verify<float>("3.14", 3.14f);
  verify<float>("-3.14", -3.14f);
  verify<float>("+3.14", +3.14f);
  verify<float>("1E+10", 1E+10f);
  verify<float>("-1E+10", -1E+10f);
  verify<float>("1.234E10", 1.234E10f);
  verify<float>("1.234E+10", 1.234E+10f);
  verify<float>("1.175494e-38", 1.175494e-38f);  // min
  verify<float>("-1.175494e-38", -1.175494e-38f);
  verify<float>("3.402823e+38", 3.402823e+38f);  // max
  verify<float>("-3.402823e+38", -3.402823e+38f);
  verify<float>("NaN", NAN);
}

TEST_F(JsonParser_Variant_Tests, String) {
  verify("\"hello world\"", "hello world");
}

TEST_F(JsonParser_Variant_Tests, True) {
  verify("true", true);
  verify("false", false);
}

TEST_F(JsonParser_Variant_Tests, OpenBrace) {
  whenInputIs("{");
  resultMustBeInvalid();
}

TEST_F(JsonParser_Variant_Tests, IncompleteStrings) {
  verify("\"", "");
  verify("\"hello", "hello");
  verify("\'", "");
  verify("\'world", "world");
}
