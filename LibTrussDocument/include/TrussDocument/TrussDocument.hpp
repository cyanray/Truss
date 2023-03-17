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
    class TrussDocument
    {
    public:
        enum class Type
        {
            Null,
            Boolean,
            Double,
            Integer,
            String,
            Object,
            Array
        };

        TrussDocument() : m_type(Type::Null) {}

        explicit TrussDocument(std::nullptr_t) : m_type(Type::Null) {}

        explicit TrussDocument(bool value) : m_type(Type::Boolean), m_value(value) {}

        explicit TrussDocument(int value) : m_type(Type::Integer), m_value(value) {}

        explicit TrussDocument(double value) : m_type(Type::Double), m_value(value) {}

        explicit TrussDocument(std::string_view value) : m_type(Type::String), m_value(string(value)) {}

        explicit TrussDocument(std::string&& value) : m_type(Type::String), m_value(std::move(value)) {}

        static TrussDocument Object();

        static TrussDocument Array();

        static TrussDocument Parse(std::string_view input);

        [[nodiscard]] Type GetValueType() const noexcept;

        TrussDocument& At(int index);

        [[nodiscard]] const TrussDocument& At(int index) const;

        TrussDocument& At(std::string_view key);

        [[nodiscard]] const TrussDocument& At(std::string_view key) const;

        TrussDocument& operator[](int index);

        const TrussDocument& operator[](int index) const;

        TrussDocument& operator[](std::string_view key);

        const TrussDocument& operator[](std::string_view key) const;


        TrussDocument& operator=(const string& value);

        TrussDocument& operator=(string&& value);

        TrussDocument& operator=(int value);

        TrussDocument& operator=(double value);

        template<typename T>
            requires std::same_as<T, bool>
        TrussDocument& operator=(T value)
        {
            this->AssignVaue(value);
            return *this;
        }

        TrussDocument& operator=(std::nullptr_t);

        TrussDocument& Add(const TrussDocument& value);

        TrussDocument& Add(TrussDocument&& value);

        template<typename T>
            requires std::is_constructible_v<TrussDocument, T>
        TrussDocument& Add(T&& value)
        {
            return Add(TrussDocument(std::forward<T>(value)));
        }

        template<typename T>
            requires std::is_assignable_v<TrussDocument, T>
        TrussDocument& Add(std::string_view key, T&& value)
        {
            return ((*this)[key] = std::forward<T>(value));
        }

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
            requires (!std::same_as<T, float>)
        [[nodiscard]] T Get() const
        {
            return get_detail<T, TValue>::Get(*this);
        }

        template<typename T>
            requires std::same_as<T, float>
        [[nodiscard]] float Get() const
        {
            return static_cast<float>(Get<double>());
        }

        template<typename T>
            requires (!std::same_as<T, float>)
        [[nodiscard]] T GetOrDefault(const T& default_value = {}) const
        {
            return get_detail<T, TValue>::GetOrDefault(*this, default_value);
        }

        template<typename T>
            requires std::same_as<T, float>
        [[nodiscard]] float GetOrDefault(const float& default_value = {}) const
        {
            return static_cast<float>(GetOrDefault<double>(default_value));
        }

        class iterator;

        [[nodiscard]] iterator begin();

        [[nodiscard]] iterator end();

    private:
        friend class ParserImpl;

        using TArray = std::vector<TrussDocument>;
        using TObject = std::map<std::string, TrussDocument>;
        using TValue = std::variant<bool, int, double, std::string, TArray, TObject>;
        Type m_type;
        TValue m_value;

        TrussDocument& AssignVaue(bool value);
    };

    class TrussDocument::iterator
    {
    private:
        friend class TrussDocument;

        iterator(TrussDocument* doc, bool is_end): m_doc(doc)
        {
            m_is_array = m_doc->IsArray();
            if (m_is_array)
            {
                auto& array = std::get<TArray>(m_doc->m_value);
                m_array_iterator = (is_end? array.end() : array.begin());
            }
            else
            {
                auto& object = std::get<TObject>(m_doc->m_value);
                m_object_iterator = (is_end? object.end() : object.begin());
            }
        }

        TrussDocument* m_doc;
        bool m_is_array{false};
        int m_array_index{0};
        TArray::iterator m_array_iterator;
        TObject::iterator m_object_iterator;

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = TrussDocument;
        using difference_type = std::ptrdiff_t;
        using pointer = TrussDocument*;
        using reference = TrussDocument&;

        iterator(const iterator&) = default;

        iterator(iterator&&) = default;

        iterator& operator=(const iterator&) = default;

        iterator& operator=(iterator&&) = default;

        iterator& operator++()
        {
            if(m_is_array)
            {
                ++m_array_iterator;
                ++m_array_index;
            }
            else
            {
                ++m_object_iterator;
            }
            return *this;
        }

        iterator operator++(int)
        {
            iterator result = *this;
            ++(*this);
            return result;
        }

        iterator& operator--()
        {
            if (m_is_array)
            {
                --m_array_iterator;
                --m_array_index;
            }
            else
            {
                --m_object_iterator;
            }
            return *this;
        }

        iterator operator--(int)
        {
            iterator result = *this;
            --(*this);
            return result;
        }

        bool operator==(const iterator& other) const
        {
            return (m_is_array ? m_array_iterator == other.m_array_iterator
                               : m_object_iterator == other.m_object_iterator);
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }

        reference operator*()
        {
            return (m_is_array ? *m_array_iterator : m_object_iterator->second);
        }

        pointer operator->()
        {
            return (m_is_array ? &(*m_array_iterator) : &m_object_iterator->second);
        }

        [[nodiscard]] int index() const
        {
            if (!m_is_array)
            {
                throw std::runtime_error("TrussDocument::iterator::index() is only valid for array");
            }
            return m_array_index;
        }

        [[nodiscard]] const std::string& key() const
        {
            if (m_is_array)
            {
                throw std::runtime_error("TrussDocument::iterator::key() is only valid for object");
            }
            return m_object_iterator->first;
        }

    };


}// namespace Truss