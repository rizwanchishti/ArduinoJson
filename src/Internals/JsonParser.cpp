// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../../include/ArduinoJson/Internals/JsonParser.hpp"

#include <stdlib.h>  // for strtol, strtod
#include <ctype.h>

#include "../../include/ArduinoJson/Internals/Encoding.hpp"
#include "../../include/ArduinoJson/JsonArray.hpp"
#include "../../include/ArduinoJson/JsonBuffer.hpp"
#include "../../include/ArduinoJson/JsonObject.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

static const char *skipSpaces(const char *ptr) {
  while (isspace(*ptr)) ptr++;
  return ptr;
}

bool JsonParser::skip(char charToSkip) {
  register const char *ptr = skipSpaces(_readPtr);
  if (*ptr != charToSkip) return false;
  ptr++;
  _readPtr = skipSpaces(ptr);
  return true;
}

bool JsonParser::skip(const char *wordToSkip) {
  register const char *ptr = _readPtr;
  while (*wordToSkip && *ptr == *wordToSkip) {
    wordToSkip++;
    ptr++;
  }
  if (*wordToSkip != '\0') return false;
  _readPtr = ptr;
  return true;
}

bool JsonParser::parseAnythingTo(JsonVariant *destination) {
  if (_nestingLimit == 0) return false;
  _nestingLimit--;
  bool success = parseAnythingToUnsafe(destination);
  _nestingLimit++;
  return success;
}

inline bool JsonParser::parseAnythingToUnsafe(JsonVariant *destination) {
  _readPtr = skipSpaces(_readPtr);

  switch (*_readPtr) {
    case '[':
      return parseArrayTo(destination);

    case '{':
      return parseObjectTo(destination);

    case 't':
    case 'f':
      return parseBooleanTo(destination);

    case '-':
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return parseNumberTo(destination);

    case 'n':
      return parseNullTo(destination);

    default:
      return parseStringTo(destination);
  }
}

JsonArray &JsonParser::parseArray() {
  // Create an empty array
  JsonArray &array = _buffer->createArray();

  // Check opening braket
  if (!skip('[')) goto ERROR_MISSING_BRACKET;
  if (skip(']')) goto SUCCESS_EMPTY_ARRAY;

  // Read each value
  for (;;) {
    // 1 - Parse value
    JsonVariant value;
    if (!parseAnythingTo(&value)) goto ERROR_INVALID_VALUE;
    if (!array.add(value)) goto ERROR_NO_MEMORY;

    // 2 - More values?
    if (skip(']')) goto SUCCES_NON_EMPTY_ARRAY;
    if (!skip(',')) goto ERROR_MISSING_COMMA;
  }

SUCCESS_EMPTY_ARRAY:
SUCCES_NON_EMPTY_ARRAY:
  return array;

ERROR_INVALID_VALUE:
ERROR_MISSING_BRACKET:
ERROR_MISSING_COMMA:
ERROR_NO_MEMORY:
  return JsonArray::invalid();
}

bool JsonParser::parseArrayTo(JsonVariant *destination) {
  JsonArray &array = parseArray();
  if (!array.success()) return false;

  *destination = array;
  return true;
}

JsonObject &JsonParser::parseObject() {
  // Create an empty object
  JsonObject &object = _buffer->createObject();

  // Check opening brace
  if (!skip('{')) goto ERROR_MISSING_BRACE;
  if (skip('}')) goto SUCCESS_EMPTY_OBJECT;

  // Read each key value pair
  for (;;) {
    // 1 - Parse key
    const char *key = parseString();
    if (!key) goto ERROR_INVALID_KEY;
    if (!skip(':')) goto ERROR_MISSING_COLON;

    // 2 - Parse value
    JsonVariant value;
    if (!parseAnythingTo(&value)) goto ERROR_INVALID_VALUE;
    if (!object.set(key, value)) goto ERROR_NO_MEMORY;

    // 3 - More keys/values?
    if (skip('}')) goto SUCCESS_NON_EMPTY_OBJECT;
    if (!skip(',')) goto ERROR_MISSING_COMMA;
  }

SUCCESS_EMPTY_OBJECT:
SUCCESS_NON_EMPTY_OBJECT:
  return object;

ERROR_INVALID_KEY:
ERROR_INVALID_VALUE:
ERROR_MISSING_BRACE:
ERROR_MISSING_COLON:
ERROR_MISSING_COMMA:
ERROR_NO_MEMORY:
  return JsonObject::invalid();
}

bool JsonParser::parseObjectTo(JsonVariant *destination) {
  JsonObject &object = parseObject();
  if (!object.success()) return false;

  *destination = object;
  return true;
}

bool JsonParser::parseBooleanTo(JsonVariant *destination) {
  if (skip("true")) {
    *destination = true;
    return true;
  } else if (skip("false")) {
    *destination = false;
    return true;
  } else {
    return false;
  }
}

bool JsonParser::parseNumberTo(JsonVariant *destination) {
  char *endOfLong;
  long longValue = strtol(_readPtr, &endOfLong, 10);
  char stopChar = *endOfLong;

  // Could it be a floating point value?
  bool couldBeFloat = stopChar == '.' || stopChar == 'e' || stopChar == 'E';

  if (couldBeFloat) {
    // Yes => parse it as a double
    double doubleValue = strtod(_readPtr, const_cast<char **>(&_readPtr));
    // Count the decimal digits
    uint8_t decimals = static_cast<uint8_t>(_readPtr - endOfLong - 1);
    // Set the variant as a double
    *destination = JsonVariant(doubleValue, decimals);
  } else {
    // No => set the variant as a long
    _readPtr = endOfLong;
    *destination = longValue;
  }
  return true;
}

bool JsonParser::parseNullTo(JsonVariant *destination) {
  const char *NULL_STRING = NULL;
  if (!skip("null")) return false;
  *destination = NULL_STRING;
  return true;
}

static bool isStopChar(char c) {
  return c == '\0' || c == ':' || c == '}' || c == ']' || c == ',';
}

const char *JsonParser::parseString() {
  const char *readPtr = _readPtr;
  char *writePtr = _writePtr;

  char c = *readPtr;

  if (c == '\'' || c == '\"') {
    char stopChar = c;
    for (;;) {
      c = *++readPtr;
      if (c == '\0') break;

      if (c == stopChar) {
        readPtr++;
        break;
      }

      if (c == '\\') {
        // replace char
        c = Encoding::unescapeChar(*++readPtr);
        if (c == '\0') break;
      }

      *writePtr++ = c;
    }
  } else {
    for (;;) {
      if (isStopChar(c)) break;
      *writePtr++ = c;
      c = *++readPtr;
    }
  }
  // end the string here
  *writePtr++ = '\0';

  const char *startPtr = _writePtr;

  // update end ptr
  _readPtr = readPtr;
  _writePtr = writePtr;

  // return pointer to unquoted string
  return startPtr;
}

bool JsonParser::parseStringTo(JsonVariant *destination) {
  const char *value = parseString();
  *destination = value;
  return value != NULL;
}
