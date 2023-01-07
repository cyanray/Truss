#pragma once
#include <string>
#include <any>
#include <map>
#include <unordered_map>
#include <memory>
#include <string_view>
using std::string;

namespace Truss
{
    enum class ValueType
    {
        Null,
        Boolean,
        Float,
        Double,
        Integer,
        String,
        Complex,
        Object,
        Array
    };

    class TrussDocument
    {
    public:
        TrussDocument(): m_type(ValueType::Null) {}

        static TrussDocument Parse(std::string_view input);

        ValueType GetValueType() const noexcept;

        TrussDocument& at(int index);
        const TrussDocument& at(int index) const;
        TrussDocument& at(std::string_view key);
        const TrussDocument& at(std::string_view key) const;
        TrussDocument& operator[](int index);
        const TrussDocument& operator[](int index) const;
        TrussDocument& operator[](std::string_view key);
        const TrussDocument& operator[](std::string_view key) const;

        bool is_object() const noexcept;
        bool is_array() const noexcept;
        size_t size() const noexcept;

        template<typename T>
        T get() const
        {
            return std::any_cast<T>(m_value);
        }

        template<>
        float get<float>() const
        {
            if (GetValueType() == ValueType::Float)
            {
                return std::any_cast<float>(m_value);
            }
            else if (GetValueType() == ValueType::Double)
            {
                return static_cast<float>(std::any_cast<double>(m_value));
            }
            throw std::bad_any_cast();
        }

        template<>
        double get<double>() const
        {
            if (GetValueType() == ValueType::Float)
            {
                return static_cast<double>(std::any_cast<float>(m_value));
            }
            else if (GetValueType() == ValueType::Double)
            {
                return std::any_cast<double>(m_value);
            }
            throw std::bad_any_cast();
        }

    private:
        friend class __ParserImpl;
        ValueType m_type;
        std::any m_value;
        std::map<int, TrussDocument> m_array;
        std::unordered_map<std::string, TrussDocument> m_object;
    };

}