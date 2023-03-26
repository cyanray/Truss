#pragma once
#include <concepts>
#include <format>
#include <ostream>
#include <string>
#include <tuple>
#include "Csv/CsvToString.hpp"
using std::string;

namespace Csv
{
    template<typename TOStream>
        requires requires(TOStream& os, const std::string& s) { os << s; }
    class CsvWriter
    {
    private:
        TOStream& m_Stream;

    public:
        explicit CsvWriter(TOStream& stream) : m_Stream(stream) {}

        template<typename... T>
        CsvWriter<TOStream>& operator<<(const std::tuple<T...>& value)
        {
            WriteTuple(value, std::index_sequence_for<T...>{});
            return *this;
        }

        template<typename... T, size_t... Index>
        void WriteTuple(const std::tuple<T...>& value, std::index_sequence<Index...>)
        {
            ((m_Stream << (Index == 0 ? "" : ",") << csv_to_string(std::get<Index>(value))), ...) << std::endl;
        }
    };

}// namespace Csv
