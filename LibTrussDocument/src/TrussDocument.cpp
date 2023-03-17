#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-dcl21-cpp"
#pragma ide diagnostic ignored "UnusedParameter"
#pragma ide diagnostic ignored "misc-no-recursion"
#include "TrussDocument/TrussDocument.hpp"
#include "TrussDocument/ParserErrorException.hpp"
#include "TrussDocument/Tokenizer.hpp"
#include <algorithm>
#include <cctype>
#include <charconv>
#include <complex>
#include <string>
using namespace std;
using namespace Truss;

namespace
{
    template<typename T>
    std::basic_string<T> lowercase(std::basic_string_view<T> s)
    {
        std::basic_string<T> s2(s);
        std::transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
        return s2;
    }

    template<typename T>
    std::basic_string<T> uppercase(std::basic_string_view<T> s)
    {
        std::basic_string<T> s2 = s;
        std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);
        return s2;
    }
}// namespace


namespace Truss
{
    class ParserImpl
    {
    public:
        static TrussDocument Parse_BooleanValue(const vector<Token>& tokens, int& current_index)
        {
            auto& token_curr = tokens[current_index++];
            TrussDocument result;
            result.m_type = TrussDocument::Type::Boolean;
            result.m_value = (token_curr.Type == TokenType::Keyword_True);
            return result;
        }

        static TrussDocument Parse_Null(const vector<Token>& tokens, int& current_index)
        {
            ++current_index;
            TrussDocument result;
            result.m_type = TrussDocument::Type::Null;
            return result;
        }

        static TrussDocument Parse_String(const vector<Token>& tokens, int& current_index)
        {
            TrussDocument result;
            result.m_type = TrussDocument::Type::String;
            result.m_value = string(*tokens[current_index++].Data);
            return result;
        }

        static TrussDocument Parse_Number(const vector<Token>& tokens, int& current_index)
        {
            auto& token_curr = tokens[current_index];
            string_view raw_number;
            bool minus = false;
            if (token_curr.Type == TokenType::Symbol_Add || token_curr.Type == TokenType::Symbol_Minus)
            {
                minus = (token_curr.Type == TokenType::Symbol_Minus);
                raw_number = *(tokens[current_index + 1]).Data;
                current_index += 2;
            }
            else
            {
                raw_number = *token_curr.Data;
                current_index += 1;
            }
            auto number_result = ParseNumberImpl(raw_number);
            int index = number_result.index;
            TrussDocument result;

            auto lambda_convert = [raw_number, index, minus]<typename T>() {
                T value{};
                from_chars_wrapper(raw_number.data(), raw_number.data() + index, value);
                return (minus ? -value : value);
            };

            if (!number_result.is_double)
            {
                result.m_type = TrussDocument::Type::Integer;
                result.m_value = lambda_convert.operator()<int>();
            }
            else
            {
                result.m_type = TrussDocument::Type::Double;
                result.m_value = lambda_convert.operator()<double>();
            }
            return result;
        }

        static TrussDocument Parse_Complex(const vector<Token>& tokens, int& current_index)
        {
            throw std::runtime_error("Not yet implemented");
        }

        static TrussDocument Parse_Object(const vector<Token>& tokens, int& current_index, bool ignore_symbol_start = false)
        {
            int index = current_index;
            if (!ignore_symbol_start)
            {
                CheckToken(tokens, index, TokenType::Symbol_Object_Start);
            }
            TrussDocument result;
            result.m_type = TrussDocument::Type::Object;
            result.m_value = TrussDocument::TObject{};
            while (true)
            {
                if (index >= tokens.size())
                {
                    if (!ignore_symbol_start) goto ERROR_HANDLING;
                    else
                        goto END;
                }
                auto& token_curr = tokens[index];
                switch (token_curr.Type)
                {
                    case TokenType::Data_Key:
                    {
                        CheckToken(tokens, ++index, TokenType::Symbol_Assignment);
                        auto value = Parse_Value(tokens, index);
                        std::get<TrussDocument::TObject>(result.m_value).insert({lowercase<char>(*token_curr.Data), value});
                        break;
                    }
                    case TokenType::Symbol_Object_End:
                        goto END;
                    default:
                        goto ERROR_HANDLING;
                }
            }
        ERROR_HANDLING:
            throw UnexpectedTokenException(TokenType::Symbol_Object_End);
        END:
            current_index = index + 1;
            return result;
        }

        static TrussDocument Parse_Array(const vector<Token>& tokens, int& current_index)
        {
            int index = current_index;
            CheckToken(tokens, index, TokenType::Symbol_Array_Start);
            TrussDocument result;
            result.m_type = TrussDocument::Type::Array;
            result.m_value = TrussDocument::TArray{};
            while (true)
            {
                if (index >= tokens.size()) goto ERROR_HANDLING;
                auto& token_curr = tokens[index];
                switch (token_curr.Type)
                {
                    case TokenType::Symbol_Array_End:
                        goto END;
                    default:
                    {
                        auto value = Parse_Value(tokens, index);
                        std::get<TrussDocument::TArray>(result.m_value).emplace_back(std::move(value));
                        break;
                    }
                }
            }
        ERROR_HANDLING:
            throw UnexpectedTokenException(TokenType::Symbol_Array_End);
        END:
            current_index = index + 1;
            return result;
        }

        static TrussDocument Parse_Value(const vector<Token>& tokens, int& current_index)
        {
            int& index = current_index;
            auto& token_curr = tokens[index];
            switch (token_curr.Type)
            {
                case TokenType::Symbol_Object_Start:
                {
                    return Parse_Object(tokens, index);
                }
                case TokenType::Symbol_Array_Start:
                {
                    return Parse_Array(tokens, index);
                }
                case TokenType::Keyword_Null:
                {
                    return Parse_Null(tokens, index);
                }
                case TokenType::Keyword_True:
                case TokenType::Keyword_False:
                {
                    return Parse_BooleanValue(tokens, index);
                }
                case TokenType::Symbol_Minus:
                case TokenType::Symbol_Add:
                case TokenType::Data_Number:
                {
                    return Parse_Number(tokens, index);
                }
                case TokenType::Symbol_Complex_Start:
                {
                    return Parse_Complex(tokens, index);
                }
                case TokenType::Data_String:
                {
                    return Parse_String(tokens, index);
                }
                default:
                {
                    // return Parse_Null(tokens, index);
                    throw UnexpectedTokenException();
                }
            }
        }

    private:
        static void CheckToken(const vector<Token>& tokens, int& current_index, TokenType type)
        {
            auto current_token = tokens[current_index].Type;
            if (current_token == type)
            {
                ++current_index;
                return;
            }
            throw Truss::UnexpectedTokenException(type, current_token);
        }

        template<typename T>
        static void from_chars_wrapper(const char* begin, const char* end, T& value)
        {
            auto r = std::from_chars(begin, end, value);
            if (r.ec != std::errc{})
            {
                throw ParseNumberException(std::string_view(begin, end));
            }
        }

        struct ParseNumberResult
        {
            int index{0};
            bool is_double{false};
        };

        static ParseNumberResult ParseNumberImpl(std::string_view raw_number)
        {
            ParseNumberResult result;
            const char* raw_number_ptr = raw_number.data();
            bool flag_decimal = false, cond_decimal;
            int index = -1;
            char ch;
            do
            {
                ch = raw_number_ptr[++index];
                cond_decimal = (ch == '.' || ch == 'e' || ch == 'E');
                flag_decimal = (flag_decimal || cond_decimal);
            } while (std::isdigit(ch) || cond_decimal || ch == '+' || ch == '-');
            result.index = index;
            result.is_double = flag_decimal;
            return result;
        }
    };

}// namespace Truss

TrussDocument TrussDocument::Parse(std::string_view input)
{
    Tokenizer tokenizer(input);
    const vector<Token> tokens = tokenizer.Scan();
    int token_index = 0;
    TrussDocument result = ParserImpl::Parse_Object(tokens, token_index, true);
    return result;
}

TrussDocument::Type TrussDocument::GetValueType() const noexcept
{
    return m_type;
}

TrussDocument& TrussDocument::At(int index)
{
    return std::get<TArray>(m_value).at(index);
}

TrussDocument& TrussDocument::At(std::string_view key)
{
    return std::get<TObject>(m_value).at(lowercase(key));
}

TrussDocument& TrussDocument::operator[](int index)
{
    return std::get<TArray>(m_value)[index];
}

TrussDocument& TrussDocument::operator[](std::string_view key)
{
    return std::get<TObject>(m_value)[lowercase(key)];
}

const TrussDocument& TrussDocument::At(int index) const
{
    return const_cast<TrussDocument*>(this)->operator[](index);
}

const TrussDocument& TrussDocument::At(std::string_view key) const
{
    return const_cast<TrussDocument*>(this)->At(key);
}

const TrussDocument& TrussDocument::operator[](int index) const
{
    return const_cast<TrussDocument*>(this)->At(index);
}

const TrussDocument& TrussDocument::operator[](std::string_view key) const
{
    return const_cast<TrussDocument*>(this)->operator[](key);
}

bool TrussDocument::IsArray() const noexcept
{
    return (GetValueType() == Type::Array);
}

bool TrussDocument::IsObject() const noexcept
{
    return (GetValueType() == Type::Object);
}

bool TrussDocument::IsValue() const noexcept
{
    return !(IsArray() && IsObject());
}

size_t TrussDocument::Count() const noexcept
{
    if (m_type == Type::Array)
    {
        return std::get<TArray>(m_value).size();
    }
    else if (m_type == Type::Object)
    {
        return std::get<TObject>(m_value).size();
    }
    return 0;
}

bool TrussDocument::Exists(const string& key) const
{
    return std::get<TObject>(m_value).contains(key);
}

bool TrussDocument::IsNull() const noexcept
{
    return (GetValueType() == Type::Null);
}

bool TrussDocument::IsBoolean() const noexcept
{
    return (GetValueType() == Type::Boolean);
}

bool TrussDocument::IsInteger() const noexcept
{
    return (GetValueType() == Type::Integer);
}

bool TrussDocument::IsDouble() const noexcept
{
    return (GetValueType() == Type::Double);
}

bool TrussDocument::IsString() const noexcept
{
    return (GetValueType() == Type::String);
}

TrussDocument& TrussDocument::operator=(const string& value)
{
    m_type = Type::String;
    m_value = value;
    return *this;
}

TrussDocument& TrussDocument::operator=(string&& value)
{
    m_type = Type::String;
    m_value = std::move(value);
    return *this;
}

TrussDocument& TrussDocument::operator=(int value)
{
    m_type = Type::Integer;
    m_value = value;
    return *this;
}

TrussDocument& TrussDocument::operator=(double value)
{
    m_type = Type::Double;
    m_value = value;
    return *this;
}

TrussDocument& TrussDocument::AssignVaue(bool value)
{
    m_type = Type::Boolean;
    m_value = value;
    return *this;
}

TrussDocument& TrussDocument::operator=(std::nullptr_t)
{
    m_type = Type::Null;
    return *this;
}

TrussDocument TrussDocument::Object()
{
    TrussDocument doc;
    doc.m_type = Type::Object;
    doc.m_value = TObject{};
    return doc;
}

TrussDocument TrussDocument::Array()
{
    TrussDocument doc;
    doc.m_type = Type::Array;
    doc.m_value = TArray{};
    return doc;
}

TrussDocument& TrussDocument::Add(const TrussDocument& value)
{
    std::get<TArray>(m_value).emplace_back(value);
    return std::get<TArray>(m_value).back();
}

TrussDocument& TrussDocument::Add(TrussDocument&& value)
{
    std::get<TArray>(m_value).emplace_back(std::move(value));
    return std::get<TArray>(m_value).back();
}

TrussDocument::iterator TrussDocument::begin()
{
    return {this, false};
}

TrussDocument::iterator TrussDocument::end()
{
    return {this, true};
}

namespace Truss
{
    TrussDocument::iterator::iterator(TrussDocument* doc, bool is_end) : m_doc(doc)
    {
        m_is_array = m_doc->IsArray();
        if (m_is_array)
        {
            auto& array = std::get<TArray>(m_doc->m_value);
            m_array_iterator = (is_end ? array.end() : array.begin());
        }
        else
        {
            auto& object = std::get<TObject>(m_doc->m_value);
            m_object_iterator = (is_end ? object.end() : object.begin());
        }
    }

    TrussDocument::iterator& TrussDocument::iterator::operator++()
    {
        if (m_is_array)
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

    TrussDocument::iterator TrussDocument::iterator::operator++(int)
    {
        iterator result = *this;
        ++(*this);
        return result;
    }

    TrussDocument::iterator& TrussDocument::iterator::operator--()
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

    TrussDocument::iterator TrussDocument::iterator::operator--(int)
    {
        iterator result = *this;
        --(*this);
        return result;
    }

    bool TrussDocument::iterator::operator==(const TrussDocument::iterator& other) const
    {
        return (m_is_array ? m_array_iterator == other.m_array_iterator
                           : m_object_iterator == other.m_object_iterator);
    }

    bool TrussDocument::iterator::operator!=(const TrussDocument::iterator& other) const
    {
        return !(*this == other);
    }

    TrussDocument& TrussDocument::iterator::operator*()
    {
        return (m_is_array ? *m_array_iterator : m_object_iterator->second);
    }

    TrussDocument::iterator::pointer TrussDocument::iterator::operator->()
    {
        return (m_is_array ? &(*m_array_iterator) : &m_object_iterator->second);
    }

    int TrussDocument::iterator::index() const
    {
        if (!m_is_array)
        {
            throw std::runtime_error("TrussDocument::iterator::index() is only valid for array");
        }
        return m_array_index;
    }

    const std::string& TrussDocument::iterator::key() const
    {
        if (m_is_array)
        {
            throw std::runtime_error("TrussDocument::iterator::key() is only valid for object");
        }
        return m_object_iterator->first;
    }

}// namespace Truss


#pragma clang diagnostic pop
