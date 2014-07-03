#include "CppUnitTest.h"
#include "JsonArray.h"
#include "JsonHashTable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonGeneratorTests
{
    TEST_CLASS(JsonHashTableTests)
    {
        JsonHashTable<2> hash;
        char buffer[256];

    public:
        
        TEST_METHOD(Empty)
        {
            returnValueIs(2);
            jsonIs("{}");
        }

        TEST_METHOD(OneString)
        {
            addValue("key", "value");

            returnValueIs(15);
            jsonIs("{\"key\":\"value\"}");
        }

        TEST_METHOD(TwoStrings)
        {
            addValue("key1", "value1");
            addValue("key2", "value2");

            returnValueIs(33);
            jsonIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(OneStringOverCapacity)
        {
            addValue("key1", "value1");
            addValue("key2", "value2");
            addValue("key3", "value3");

            returnValueIs(33);
            jsonIs("{\"key1\":\"value1\",\"key2\":\"value2\"}");
        }

        TEST_METHOD(OneInteger)
        {
            addValue("key", 1);

            returnValueIs(9);
            jsonIs("{\"key\":1}");
        }

        TEST_METHOD(OneNull)
        {
            addValue("key", (char*) 0);

            returnValueIs(12);
            jsonIs("{\"key\":null}");
        }

        TEST_METHOD(OneTrue)
        {
            addValue("key", true);

            returnValueIs(12);
            jsonIs("{\"key\":true}");
        }

        TEST_METHOD(OneFalse)
        {
            addValue("key", false);

            returnValueIs(13);
            jsonIs("{\"key\":false}");
        }

        TEST_METHOD(OneEmptyNestedArray)
        {
            JsonArray<1> nestedArray;
            addNested("key", nestedArray);

            returnValueIs(10);
            jsonIs("{\"key\":[]}");
        }

        TEST_METHOD(OneEmptyNestedHash)
        {
            JsonHashTable<1> nestedHash;
            addNested("key", nestedHash);

            returnValueIs(10);
            jsonIs("{\"key\":{}}");
        }

    private:
        
        void addNested(const char* key, JsonObjectBase& value)
        {
            hash.add(key, value);
        }

        template<typename T>
        void addValue(const char* key, T value)
        {
            hash.add(key, value);
        }

        void jsonIs(const char* expected)
        {            
            hash.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, buffer);
        }

        void returnValueIs(size_t expected)
        {
            size_t actual = hash.printTo(buffer, sizeof(buffer));
            Assert::AreEqual(expected, actual);
        }
    };
}