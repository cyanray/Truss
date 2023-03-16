#pragma once

#include <complex>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

using std::string;


namespace Truss
{
    enum class ValueType
    {
        Null,
        Boolean,
        Double,
        Integer,
        String,
        Object,
        Array
    };

    class TrussDocument
    {
    public:
        TrussDocument() : m_type(ValueType::Null) {}

        static TrussDocument Parse(std::string_view input);

        [[nodiscard]] ValueType GetValueType() const noexcept;

        TrussDocument& At(int index);

        [[nodiscard]] const TrussDocument& At(int index) const;

        TrussDocument& At(std::string_view key);

        [[nodiscard]] const TrussDocument& At(std::string_view key) const;

        TrussDocument& operator[](int index);

        const TrussDocument& operator[](int index) const;

        TrussDocument& operator[](std::string_view key);

        const TrussDocument& operator[](std::string_view key) const;

        [[nodiscard]] bool Exists(const string& key) const;

        [[nodiscard]] bool IsNull() const noexcept;

        [[nodiscard]] bool IsBoolean() const noexcept;

        [[nodiscard]] bool IsDouble() const noexcept;

        [[nodiscard]] bool IsInteger() const noexcept;

        [[nodiscard]] bool IsString() const noexcept;

        [[nodiscard]] bool IsValue() const noexcept;

        [[nodiscard]] bool IsObject() const noexcept;

        [[nodiscard]] bool IsArray() const noexcept;

        [[nodiscard]] size_t Count() const noexcept;

        template<typename T, typename... All_T>
        struct get_detail
        {
            static T Get(const TrussDocument& doc)
            {
                return std::get<T>(doc.m_value);
            }

            static T GetOrDefault(const TrussDocument& doc, const T& default_value)
            {
                const T* value_ptr = std::get_if<T>(&doc.m_value);
                if (value_ptr != nullptr) return *value_ptr;
                return default_value;
            }
        };

        template<typename T, typename... All_T>
            requires(!std::same_as<T, All_T> && ...)
        struct get_detail<T, std::variant<All_T...>>
        {
            static T Get(const TrussDocument& doc)
            {
                T result{};
                from_truss(doc, result);
                return result;
            }

            static T GetOrDefault(const TrussDocument& doc, const T& default_value)
            {
                try
                {
                    return Get(doc);
                }
                catch (...)
                {
                    return default_value;
                }
            }
        };

        template<typename T>
        [[nodiscard]] T Get() const
        {
            return get_detail<T, ValueContainer>::Get(*this);
        }

        template<typename T>
            requires std::same_as<T, float>
        [[nodiscard]] float Get() const
        {
            return static_cast<float>(Get<double>());
        }

        template<typename T>
        [[nodiscard]] T GetOrDefault(const T& default_value = {}) const
        {
            return get_detail<T, ValueContainer>::GetOrDefault(*this, default_value);
        }

        template<typename T>
            requires std::same_as<T, float>
        [[nodiscard]] float GetOrDefault(const float& default_value = {}) const
        {
            return static_cast<float>(GetOrDefault<double>(default_value));
        }

    private:
        friend class ParserImpl;

        using ValueContainer = std::variant<bool, int, double, std::string>;
        ValueType m_type;
        ValueContainer m_value;
        std::vector<TrussDocument> m_array;
        std::map<std::string, TrussDocument> m_object;
    };

}// namespace Truss