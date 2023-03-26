#pragma once
#include <concepts>
#include <format>
#include <string>
#include <string_view>

namespace Csv
{
    template<typename T>
        requires std::floating_point<T>
    inline std::string csv_to_string(T value)
    {
        return std::format("{:G}", T(value));
    }

    template<typename T>
        requires std::integral<T>
    inline std::string csv_to_string(T value)
    {
        return std::to_string(value);
    }

    inline std::string csv_to_string(const std::string& value)
    {
        // TODO: escape double quotes
        return value;
    }

}