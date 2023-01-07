#include <string>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <complex>
#include "TrussDocument/TrussDocument.hpp"
#include "TrussDocument/Tokenizer.hpp"
using namespace std;
using namespace Truss;

namespace
{
    template <typename T>
    std::basic_string<T> lowercase(std::basic_string_view<T> s)
    {
        std::basic_string<T> s2(s);
        std::transform(s2.begin(), s2.end(), s2.begin(), tolower);
        return s2;
    }

    template <typename T>
    std::basic_string<T> uppercase(std::basic_string_view<T> s)
    {
        std::basic_string<T> s2 = s;
        std::transform(s2.begin(), s2.end(), s2.begin(), toupper);
        return s2;
    }
}


ValueType TrussDocument::GetValueType() const noexcept
{
    return m_type;
}

TrussDocument &TrussDocument::at(int index)
{
    return m_array.at(index);
}

TrussDocument &TrussDocument::at(std::string_view key)
{
    return m_object.at(lowercase(key));
}

TrussDocument &TrussDocument::operator[](int index)
{
    return this->at(index);
}

TrussDocument &TrussDocument::operator[](std::string_view key)
{
    return this->at(key);
}

const TrussDocument &TrussDocument::at(int index) const
{
    return const_cast<TrussDocument *>(this)->at(index);
}

const TrussDocument &TrussDocument::at(std::string_view key) const
{
    return const_cast<TrussDocument *>(this)->at(key);
}

const TrussDocument &TrussDocument::operator[](int index) const
{
    return const_cast<TrussDocument *>(this)->at(index);
}

const TrussDocument &TrussDocument::operator[](std::string_view key) const
{
    return const_cast<TrussDocument *>(this)->at(key);
}

namespace Truss
{
    class __ParserImpl;
}

class Truss::__ParserImpl
{
public:
    static TrussDocument Parse_BooleanValue(const vector<Token>& tokens, int& current_index)
    {
        auto& token_curr = tokens[current_index++];
        TrussDocument result;
        result.m_type = ValueType::Boolean;
        result.m_value = (token_curr.Type == TokenType::Keyword_True);
        return result;
    }

    static TrussDocument Parse_Null(const vector<Token>& tokens, int& current_index)
    {
        ++current_index;
        TrussDocument result;
        result.m_type = ValueType::Null;
        return result;
    }

    static TrussDocument Parse_String(const vector<Token>& tokens, int& current_index)
    {
        TrussDocument result;
        result.m_type = ValueType::String;
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
        if (!number_result.flag_decimal) [[unlikely]]
        {
            TrussDocument result;
            int value = 0;
            from_chars_wrapper(raw_number.data(), raw_number.data() + index, value);
            result.m_type = ValueType::Integer;
            result.m_value = (minus ? -value : value);
            return result;
        }

        TrussDocument result;
        if (number_result.flag_double) [[unlikely]]
        {
            double value = 0;
            from_chars_wrapper(raw_number.data(), raw_number.data() + index, value);
            result.m_type = ValueType::Double;
            result.m_value = (minus ? -value : value);
        }
        else [[likely]]
        {
            float value = 0;
            from_chars_wrapper(raw_number.data(), raw_number.data() + index, value);
            result.m_type = ValueType::Float;
            result.m_value = (minus ? -value : value);
        }
        return result;
    }

    static TrussDocument Parse_Complex(const vector<Token>& tokens, int& current_index) // TODO: 重构
    {
        int index = current_index;
        if (index + 4 >= tokens.size()) ;// TODO: Error handling
        if (tokens[index].Type != TokenType::Symbol_Complex_Start) ;// TODO: Error handling
        if (tokens[index + 1].Type != TokenType::Data_Number) ;// TODO: Error handling
        string_view number1_str = *tokens[index + 1].Data;
        auto number1_result = ParseNumberImpl(number1_str);
        if (tokens[index + 2].Type != TokenType::Symbol_Add && tokens[index + 2].Type != TokenType::Symbol_Minus) ;// TODO: Error handling
        if (tokens[index + 3].Type != TokenType::Data_Number) ;// TODO: Error handling
        string_view number2_str = *tokens[index + 3].Data;
        auto number2_result = ParseNumberImpl(number2_str);
        if (tokens[index + 4].Type != TokenType::Symbol_Complex_End) ;// TODO: Error handling
        if (number1_result.flag_double || number2_result.flag_double)
        {
            double number1, number2;
            from_chars_wrapper(number1_str.data(), number1_str.data() + number1_result.index, number1);
            from_chars_wrapper(number2_str.data(), number2_str.data() + number2_result.index, number2);
            complex<double> value(number1, number2);
            TrussDocument result;
            result.m_type = ValueType::Complex;
            result.m_value = value;
            current_index += 5;
            return result;
        }
        else
        {
            float number1, number2;
            from_chars_wrapper(number1_str.data(), number1_str.data() + number1_result.index, number1);
            from_chars_wrapper(number2_str.data(), number2_str.data() + number2_result.index, number2);
            complex<float> value(number1, number2);
            TrussDocument result;
            result.m_type = ValueType::Complex;
            result.m_value = value;
            current_index += 5;
            return result;
        }
    }

    static TrussDocument Parse_Object(const vector<Token>& tokens, int& current_index, bool ignore_symbol_start = false)
    {
        int index = current_index;
        if (!ignore_symbol_start)
        {
            ConsumeToken(tokens, index, TokenType::Symbol_Object_Start);
        }
        TrussDocument result;
        result.m_type = ValueType::Object;
        while (true)
        {
            if (index >= tokens.size())
            {
                if (!ignore_symbol_start) goto ERROR_HANDLING;
                else goto END;
            }
            auto& token_curr = tokens[index];
            switch (token_curr.Type)
            {
                case TokenType::Data_Key:
                {
                    ConsumeToken(tokens, ++index, TokenType::Symbol_Assignment);
                    auto value = ParseLiteralValue(tokens, index);
                    result.m_object.insert({ lowercase<char>(*token_curr.Data), value });
                    break;
                }
                case TokenType::Symbol_Object_End: goto END;
                default: goto ERROR_HANDLING;
            }
        }
        ERROR_HANDLING:
        // TODO: Error handling
        END:
        current_index = index + 1;
        return result;
    }

    static TrussDocument Parse_Array(const vector<Token>& tokens, int& current_index)
    {
        int index = current_index;
        ConsumeToken(tokens, index, TokenType::Symbol_Array_Start);
        TrussDocument result;
        result.m_type = ValueType::Array;
        int count = 0;
        while (true)
        {
            if (index >= tokens.size()) goto ERROR_HANDLING;
            auto& token_curr = tokens[index];
            switch (token_curr.Type)
            {
            case TokenType::Symbol_Array_End: goto END;
            default:
            {
                auto value = ParseLiteralValue(tokens, index);
                result.m_array.insert({ count++, value });
                break;
            }
            }
        }
    ERROR_HANDLING:
        // TODO: Error handling
    END:
        current_index = index + 1;
        return result;
    }

    static TrussDocument ParseLiteralValue(const vector<Token>& tokens, int& current_index)
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
                return Parse_Null(tokens, index);    // TODO: error handling
            }
        }
    }

private:

    static void ConsumeToken(const vector<Token>& tokens, int& current_index, TokenType type)
    {
        if (tokens[current_index].Type == type)
        {
            ++current_index;
            return;
        }
        // TODO: error handling
    }

    template<typename T>
    static void from_chars_wrapper(const char *begin, const char *end, T &value)
    {
        auto r = std::from_chars(begin, end, value);
        if (r.ec != std::errc{})
        {
            // TODO: Error handling
        }
    }

    struct ParseNumberResult
    {
        int index = 0;
        bool flag_decimal = false;
        bool flag_float = false;
        bool flag_double = false;
        bool flag_complex_imaginary = false;
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
        result.flag_decimal = flag_decimal;
        for (int i = index; i < raw_number.size(); ++i)
        {
            char suffix = raw_number_ptr[i];
            result.flag_float = (suffix == 'f');
            result.flag_double = (suffix == 'd');
            result.flag_complex_imaginary = (suffix == 'i');
        }
        return result;
    }
};


TrussDocument TrussDocument::Parse(std::string_view input)
{
    Tokenizer tokenizer(input);
    const vector<Token> tokens = tokenizer.Scan();
    int token_index = 0;
    TrussDocument result = __ParserImpl::Parse_Object(tokens, token_index, true);
    return result;
}

bool TrussDocument::is_array() const noexcept
{
    return (GetValueType() == ValueType::Array);
}

bool TrussDocument::is_object() const noexcept
{
    return (GetValueType() == ValueType::Object);
}

size_t TrussDocument::size() const noexcept
{
    return m_array.size();
}

