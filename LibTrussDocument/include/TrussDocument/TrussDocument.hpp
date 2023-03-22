#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-dcl21-cpp"
#pragma once

#include <complex>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "TrussDocumentFormatter.hpp"

using std::string;


namespace Truss
{
    class TrussDocument;
    template<typename T>
    concept HasFromTruss = requires(const TrussDocument& d, T a) { from_truss(d, a); };

    template<typename T>
    concept HasToTruss = requires(TrussDocument& d, const T& a) { to_truss(d, a); };

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

    private:
        friend class ParserImpl;

        using TArray = std::vector<TrussDocument>;
        using TObject = std::map<std::string, TrussDocument>;
        using TValue = std::variant<bool, int, double, std::string, TArray, TObject>;
        Type m_type;
        TValue m_value;

        TrussDocument& AssignVaue(bool value);

    public:
        TrussDocument() : m_type(Type::Object), m_value(TObject{}) {}

        explicit TrussDocument(std::nullptr_t) : m_type(Type::Null) {}

        explicit TrussDocument(bool value) : m_type(Type::Boolean), m_value(value) {}

        explicit TrussDocument(int value) : m_type(Type::Integer), m_value(value) {}

        explicit TrussDocument(double value) : m_type(Type::Double), m_value(value) {}

        explicit TrussDocument(std::string_view value) : m_type(Type::String), m_value(string(value)) {}

        explicit TrussDocument(std::string&& value) : m_type(Type::String), m_value(std::move(value)) {}

        template<HasToTruss T>
        explicit TrussDocument(const T& value) : m_type(Type::Object), m_value(TObject{})
        {
            to_truss(*this, value);
        }

        static TrussDocument Object();

        static TrussDocument Array();

        static TrussDocument Parse(std::string_view input);

        void InitObject();

        void InitArray();

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
        };

        template<typename T, typename... All_T>
            requires(std::same_as<T, All_T> || ...)
        struct get_detail<T, std::variant<All_T...>>
        {
            using type = bool;

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
            requires((!std::same_as<T, All_T> && ...) && HasFromTruss<T>)
        struct get_detail<T, std::variant<All_T...>>
        {
            using type = bool;

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
            requires std::same_as<bool, typename get_detail<T, TValue>::type>
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
            requires std::same_as<T, std::vector<typename T::value_type>>
        [[nodiscard]] T Get() const
        {
            using U = typename T::value_type;
            T result{};
            result.reserve(Count());
            for (const auto& item: std::get<TArray>(m_value))
            {
                result.push_back(item.Get<U>());
            }
            return result;
        }

        template<typename T>
            requires std::same_as<bool, typename get_detail<T, TValue>::type>
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

        [[nodiscard]] string ToString(const TrussDocumentFormatter& f = {}) const;

        class iterator_base;
        template<typename T>
        class iterator_impl;
        using iterator = iterator_impl<TrussDocument>;
        using const_iterator = iterator_impl<const TrussDocument>;

        [[nodiscard]] iterator begin();
        [[nodiscard]] iterator end();

        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] const_iterator end() const;
    };

    class TrussDocument::iterator_base
    {
    protected:
        friend class TrussDocument;
        TrussDocument* m_doc;
        bool m_is_array{false};
        int m_array_index{0};
        TArray::iterator m_array_iterator;
        TObject::iterator m_object_iterator;
        iterator_base(TrussDocument* doc, bool is_end);

    public:
        iterator_base(const iterator_base&) = default;
        iterator_base(iterator_base&&) = default;
        iterator_base& operator=(const iterator_base&) = default;
        iterator_base& operator=(iterator_base&&) = default;
        iterator_base& operator++();
        iterator_base operator++(int);
        iterator_base& operator--();
        iterator_base operator--(int);
        bool operator==(const iterator_base& other) const;
        bool operator!=(const iterator_base& other) const;

        [[nodiscard]] int index() const;
        [[nodiscard]] const std::string& key() const;
    };

    template<typename T>
    class TrussDocument::iterator_impl : public TrussDocument::iterator_base
    {
    private:
        friend class TrussDocument;

        iterator_impl(T* doc, bool is_end)
            : iterator_base(const_cast<std::remove_cv_t<T>*>(doc), is_end)
        {
        }

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator_impl(const iterator_impl<T>&) = default;
        iterator_impl(iterator_impl<T>&&) noexcept = default;
        iterator_impl<T>& operator=(const iterator_impl<T>&) = default;
        iterator_impl<T>& operator=(iterator_impl<T>&&) noexcept = default;

        [[nodiscard]] T& operator*() const
        {
            return (m_is_array ? *m_array_iterator : m_object_iterator->second);
        }

        [[nodiscard]] T* operator->() const
        {
            return &(operator*());
        }
    };


}// namespace Truss
#pragma clang diagnostic pop