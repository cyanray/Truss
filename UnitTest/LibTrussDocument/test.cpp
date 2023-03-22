#include <TrussDocument/TrussDocument.hpp>
#include <catch2/catch_test_macros.hpp>
#include <array>
#include <vector>
using namespace Truss;


TEST_CASE("Parsing numbers", "[ParseNumbers]")
{
    SECTION("Parse integer")
    {
        TrussDocument doc = TrussDocument::Parse("value:123");
        auto& value = doc["value"];
        REQUIRE(value.IsInteger());
        REQUIRE(value.Get<int>() == 123);
    }

    SECTION("Parse negative integer")
    {
        TrussDocument doc = TrussDocument::Parse("value:-123");
        auto& value = doc["value"];
        REQUIRE(value.IsInteger());
        REQUIRE(value.Get<int>() == -123);
    }

    SECTION("Parse double")
    {
        TrussDocument doc = TrussDocument::Parse("value:123.456");
        auto& value = doc["value"];
        REQUIRE(value.IsDouble());
        REQUIRE(value.Get<double>() == 123.456);
    }
    SECTION("Parse double with exponent(e-)")
    {
        TrussDocument doc = TrussDocument::Parse("value:123.456e-10");
        auto& value = doc["value"];
        REQUIRE(value.IsDouble());
        REQUIRE(value.Get<double>() == 123.456e-10);
    }

    SECTION("Parse double with exponent(e+)")
    {
        TrussDocument doc = TrussDocument::Parse("value:123.456e+10");
        auto& value = doc["value"];
        REQUIRE(value.IsDouble());
        REQUIRE(value.Get<double>() == 123.456e+10);
    }

    SECTION("Parse double with exponent(e)")
    {
        TrussDocument doc = TrussDocument::Parse("value:123.456e10");
        auto& value = doc["value"];
        REQUIRE(value.IsDouble());
        REQUIRE(value.Get<double>() == 123.456e10);
    }

    SECTION("Parse double with exponent(E)")
    {
        TrussDocument doc = TrussDocument::Parse("value:123.456E10");
        auto& value = doc["value"];
        REQUIRE(value.IsDouble());
        REQUIRE(value.Get<double>() == 123.456e10);
    }

    SECTION("Parse double with exponent(E+)")
    {
        TrussDocument doc = TrussDocument::Parse("value:123.456E+10");
        auto& value = doc["value"];
        REQUIRE(value.IsDouble());
        REQUIRE(value.Get<double>() == 123.456e+10);
    }

    SECTION("Parse double with exponent(E-)")
    {
        TrussDocument doc = TrussDocument::Parse("value:123.456E-10");
        auto& value = doc["value"];
        REQUIRE(value.IsDouble());
        REQUIRE(value.Get<double>() == 123.456e-10);
    }
}

TEST_CASE("Parsing object", "[ParseObject]")
{

    SECTION("Parse empty document")
    {
        TrussDocument doc = TrussDocument::Parse("");
        REQUIRE(doc.Count() == 0);
    }

    SECTION("Parse empty object")
    {
        TrussDocument doc = TrussDocument::Parse("value:{}");
        auto& value = doc["value"];
        REQUIRE(value.IsObject());
        REQUIRE(value.Count() == 0);
    }

    SECTION("Parse object with one key-value pair")
    {
        TrussDocument doc = TrussDocument::Parse("value:{ key:123 }");
        auto& value = doc["value"];
        REQUIRE(value.IsObject());
        REQUIRE(value.Count() == 1);
        REQUIRE(value["key"].IsInteger());
        REQUIRE(value["key"].Get<int>() == 123);
    }

    SECTION("Parse object with multiple key-value pairs")
    {
        TrussDocument doc = TrussDocument::Parse("value:{ key1:123, key2:456, key3:789 }");
        auto& value = doc["value"];
        REQUIRE(value.IsObject());
        REQUIRE(value.Count() == 3);
        REQUIRE(value["key1"].IsInteger());
        REQUIRE(value["key1"].Get<int>() == 123);
        REQUIRE(value["key2"].IsInteger());
        REQUIRE(value["key2"].Get<int>() == 456);
        REQUIRE(value["key3"].IsInteger());
        REQUIRE(value["key3"].Get<int>() == 789);
    }
}

TEST_CASE("Parsing array", "[ParseArray]")
{
    SECTION("Parse empty array")
    {
        TrussDocument doc = TrussDocument::Parse("value:[]");
        auto& value = doc["value"];
        REQUIRE(value.IsArray());
        REQUIRE(value.Count() == 0);
    }

    SECTION("Parse array with one element")
    {
        TrussDocument doc = TrussDocument::Parse("value:[123]");
        auto& value = doc["value"];
        REQUIRE(value.IsArray());
        REQUIRE(value.Count() == 1);
        REQUIRE(value[0].IsInteger());
        REQUIRE(value[0].Get<int>() == 123);
    }

    SECTION("Parse array with multiple elements")
    {
        TrussDocument doc = TrussDocument::Parse("value:[123, 456, 789]");
        auto& value = doc["value"];
        REQUIRE(value.IsArray());
        REQUIRE(value.Count() == 3);
        REQUIRE(value[0].IsInteger());
        REQUIRE(value[0].Get<int>() == 123);
        REQUIRE(value[1].IsInteger());
        REQUIRE(value[1].Get<int>() == 456);
        REQUIRE(value[2].IsInteger());
        REQUIRE(value[2].Get<int>() == 789);
    }
}

TEST_CASE("Parsing string", "[ParseString]")
{
    SECTION("Parse empty string")
    {
        TrussDocument doc = TrussDocument::Parse("value:\"\"");
        auto& value = doc["value"];
        REQUIRE(value.IsString());
        REQUIRE(value.Get<std::string>().empty());
    }

    SECTION("Parse string with one character")
    {
        TrussDocument doc = TrussDocument::Parse("value:\"a\"");
        auto& value = doc["value"];
        REQUIRE(value.IsString());
        REQUIRE(value.Get<std::string>() == "a");
    }

    SECTION("Parse string with multiple characters")
    {
        TrussDocument doc = TrussDocument::Parse("value:\"abc\"");
        auto& value = doc["value"];
        REQUIRE(value.IsString());
        REQUIRE(value.Get<std::string>() == "abc");
    }

    SECTION("Parse string with escape character")
    {
        TrussDocument doc = TrussDocument::Parse(R"(value:"\"")");
        auto& value = doc["value"];
        REQUIRE(value.IsString());
        REQUIRE(value.Get<std::string>() == "\"");
    }
}

TEST_CASE("Parsing bool", "[ParseBoolean]")
{
    SECTION("Parse true")
    {
        TrussDocument doc = TrussDocument::Parse("value:true");
        auto& value = doc["value"];
        REQUIRE(value.IsBoolean());
        REQUIRE(value.Get<bool>() == true);
    }

    SECTION("Parse false")
    {
        TrussDocument doc = TrussDocument::Parse("value:false");
        auto& value = doc["value"];
        REQUIRE(value.IsBoolean());
        REQUIRE(value.Get<bool>() == false);
    }
}

TEST_CASE("Parsing null", "[ParseNull]")
{
    SECTION("Parse null")
    {
        TrussDocument doc = TrussDocument::Parse("value:null");
        auto& value = doc["value"];
        REQUIRE(value.IsNull());
    }
}

TEST_CASE("Misc", "[Misc]")
{
    SECTION("Documents with no visible characters")
    {
        TrussDocument doc = TrussDocument::Parse(" \n  value:123 \r\n \n\r \t, val:456.0 \t \r \t");
        REQUIRE(doc["value"].IsInteger());
        REQUIRE(doc["value"].Get<int>() == 123);
        REQUIRE(doc["val"].IsDouble());
        REQUIRE(doc["val"].Get<double>() == 456.0);
    }

    SECTION("Get float while value is double")
    {
        TrussDocument doc = TrussDocument::Parse("value:123.456");
        auto& value = doc["value"];
        REQUIRE(value.IsDouble());
        REQUIRE(value.Get<float>() == 123.456f);
    }
}

TEST_CASE("Assignment operator", "[AssignmentOperator]")
{
    SECTION("Assigning null")
    {
        TrussDocument doc = TrussDocument::Object();
        doc["value"] = nullptr;
        REQUIRE(doc["value"].IsNull());
    }

    SECTION("Assigning int")
    {
        TrussDocument doc = TrussDocument::Object();
        doc["value"] = 123;
        REQUIRE(doc["value"].IsInteger());
        REQUIRE(doc["value"].Get<int>() == 123);
    }

    SECTION("Assigning double")
    {
        TrussDocument doc = TrussDocument::Object();
        doc["value"] = 123.456;
        REQUIRE(doc["value"].IsDouble());
        REQUIRE(doc["value"].Get<double>() == 123.456);
    }

    SECTION("Assigning const string&")
    {
        string str = "abc";
        TrussDocument doc = TrussDocument::Object();
        doc["value"] = str;
        REQUIRE(doc["value"].IsString());
        REQUIRE(doc["value"].Get<std::string>() == str);
    }

    SECTION("Assigning string&&")
    {
        TrussDocument doc = TrussDocument::Object();
        doc["value"] = string("abc");
        REQUIRE(doc["value"].IsString());
        REQUIRE(doc["value"].Get<std::string>() == "abc");
    }

    SECTION("Assigning const char(&)[]")
    {
        TrussDocument doc = TrussDocument::Object();
        doc["value"] = "abc";
        REQUIRE(doc["value"].IsString());
        REQUIRE(doc["value"].Get<std::string>() == "abc");
    }

    SECTION("Assigning bool(true)")
    {
        TrussDocument doc = TrussDocument::Object();
        doc["value"] = true;
        REQUIRE(doc["value"].IsBoolean());
        REQUIRE(doc["value"].Get<bool>() == true);
    }

    SECTION("Assigning bool(false)")
    {
        TrussDocument doc = TrussDocument::Object();
        doc["value"] = false;
        REQUIRE(doc["value"].IsBoolean());
        REQUIRE(doc["value"].Get<bool>() == false);
    }
}

TEST_CASE("For array", "[ForArray]")
{
    SECTION("Push back TrussDocument&&")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(TrussDocument(123));
        doc.Add(TrussDocument(456));
        REQUIRE(doc[0].IsInteger());
        REQUIRE(doc[0].Get<int>() == 123);
        REQUIRE(doc[1].IsInteger());
        REQUIRE(doc[1].Get<int>() == 456);
    }

    SECTION("Push back const TrussDocument&")
    {
        TrussDocument doc = TrussDocument::Array();
        TrussDocument doc1 = TrussDocument(123);
        TrussDocument doc2 = TrussDocument(456);
        doc.Add(doc1);
        doc.Add(doc2);
        REQUIRE(doc[0].IsInteger());
        REQUIRE(doc[0].Get<int>() == 123);
        REQUIRE(doc[1].IsInteger());
        REQUIRE(doc[1].Get<int>() == 456);
    }

    SECTION("Push back int")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(123);
        doc.Add(456);
        REQUIRE(doc[0].IsInteger());
        REQUIRE(doc[0].Get<int>() == 123);
        REQUIRE(doc[1].IsInteger());
        REQUIRE(doc[1].Get<int>() == 456);
    }

    SECTION("Push back double")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(123.456);
        doc.Add(789.012);
        REQUIRE(doc[0].IsDouble());
        REQUIRE(doc[0].Get<double>() == 123.456);
        REQUIRE(doc[1].IsDouble());
        REQUIRE(doc[1].Get<double>() == 789.012);
    }

    SECTION("Push back const string&")
    {
        TrussDocument doc = TrussDocument::Array();
        string str1 = "abc";
        string str2 = "def";
        doc.Add(str1);
        doc.Add(str2);
        REQUIRE(doc[0].IsString());
        REQUIRE(doc[0].Get<std::string>() == str1);
        REQUIRE(doc[1].IsString());
        REQUIRE(doc[1].Get<std::string>() == str2);
    }

    SECTION("Push back string&&")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(string("abc"));
        doc.Add(string("def"));
        REQUIRE(doc[0].IsString());
        REQUIRE(doc[0].Get<std::string>() == "abc");
        REQUIRE(doc[1].IsString());
        REQUIRE(doc[1].Get<std::string>() == "def");
    }

    SECTION("Push back bool")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(true);
        doc.Add(false);
        REQUIRE(doc[0].IsBoolean());
        REQUIRE(doc[0].Get<bool>() == true);
        REQUIRE(doc[1].IsBoolean());
        REQUIRE(doc[1].Get<bool>() == false);
    }

    SECTION("Iterator test (++it)")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(123);
        doc.Add(456);
        doc.Add(789);
        int i = 0;
        for (auto it = doc.begin(); it != doc.end(); ++it)
        {
            REQUIRE(it->IsInteger());
            REQUIRE(it.index() == i);
            REQUIRE(it->Get<int>() == 123 + i * 333);
            i++;
        }
    }

    SECTION("Iterator test (it++)")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(123);
        doc.Add(456);
        doc.Add(789);
        int i = 0;
        for (auto it = doc.begin(); it != doc.end(); it++)
        {
            REQUIRE(it->IsInteger());
            REQUIRE(it.index() == i);
            REQUIRE(it->Get<int>() == 123 + i * 333);
            i++;
        }
    }

    SECTION("Iterator test range-base")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(123);
        doc.Add(456);
        doc.Add(789);
        int i = 0;
        for (auto& it : doc)
        {
            REQUIRE(it.IsInteger());
            REQUIRE(it.Get<int>() == 123 + i * 333);
            i++;
        }
    }

    SECTION("Iterator test range-base const")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(123);
        doc.Add(456);
        doc.Add(789);
        int i = 0;
        for (const auto& it : doc)
        {
            REQUIRE(it.IsInteger());
            REQUIRE(it.Get<int>() == 123 + i * 333);
            i++;
        }
    }

    SECTION("Iterator test range-base of const TrussDocument")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(123);
        doc.Add(456);
        doc.Add(789);
        const TrussDocument& doc2 = doc;
        int i = 0;
        for (const auto& it : doc2)
        {
            REQUIRE(it.IsInteger());
            REQUIRE(it.Get<int>() == 123 + i * 333);
            i++;
        }
    }

    SECTION("Get std::vector<T> from TrussDocument::Array()")
    {
        TrussDocument doc = TrussDocument::Array();
        doc.Add(123);
        doc.Add(456);
        doc.Add(789);
        auto vec = doc.Get<std::vector<int>>();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec[0] == 123);
        REQUIRE(vec[1] == 456);
        REQUIRE(vec[2] == 789);
    }


}


TEST_CASE("For object", "[ForObject]")
{
    SECTION("Add key value pair into object")
    {

        TrussDocument doc = TrussDocument::Object();
        doc.Add("key1", 123);
        doc.Add("key2", "abc");
        doc.Add("key3", true);
        doc.Add("key4", 456.789);
        REQUIRE(doc["key1"].IsInteger());
        REQUIRE(doc["key1"].Get<int>() == 123);
        REQUIRE(doc["key2"].IsString());
        REQUIRE(doc["key2"].Get<std::string>() == "abc");
        REQUIRE(doc["key3"].IsBoolean());
        REQUIRE(doc["key3"].Get<bool>() == true);
        REQUIRE(doc["key4"].IsDouble());
        REQUIRE(doc["key4"].Get<double>() == 456.789);
    }

    SECTION("Iterator test (++it)")
    {
        TrussDocument doc = TrussDocument::Object();
        doc.Add("key1", 123);
        doc.Add("key2", 456);
        doc.Add("key3", 789);
        std::array key_list = { "key1", "key2", "key3"};
        int i = 0;
        for (auto it = doc.begin(); it != doc.end(); ++it)
        {
            REQUIRE(it->IsInteger());
            REQUIRE(it.key() == key_list[i]);
            REQUIRE(it->Get<int>() == 123 + i * 333);
            i++;
        }
    }

    SECTION("Iterator test (it++)")
    {
        TrussDocument doc = TrussDocument::Object();
        doc.Add("key1", 123);
        doc.Add("key2", 456);
        doc.Add("key3", 789);
        std::array key_list = { "key1", "key2", "key3"};
        int i = 0;
        for (auto it = doc.begin(); it != doc.end(); it++)
        {
            REQUIRE(it->IsInteger());
            REQUIRE(it.key() == key_list[i]);
            REQUIRE(it->Get<int>() == 123 + i * 333);
            i++;
        }
    }

    SECTION("Iterator test range-base")
    {
        TrussDocument doc = TrussDocument::Object();
        doc.Add("key1", 123);
        doc.Add("key2", 456);
        doc.Add("key3", 789);
        int i = 0;
        for (auto& it : doc)
        {
            REQUIRE(it.IsInteger());
            REQUIRE(it.Get<int>() == 123 + i * 333);
            i++;
        }
    }

    SECTION("Iterator test range-base const")
    {
        TrussDocument doc = TrussDocument::Object();
        doc.Add("key1", 123);
        doc.Add("key2", 456);
        doc.Add("key3", 789);
        int i = 0;
        for (const auto& it : doc)
        {
            REQUIRE(it.IsInteger());
            REQUIRE(it.Get<int>() == 123 + i * 333);
            i++;
        }
    }


}
