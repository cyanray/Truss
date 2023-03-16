#include <TrussDocument/TrussDocument.hpp>
#include <catch2/catch_test_macros.hpp>
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