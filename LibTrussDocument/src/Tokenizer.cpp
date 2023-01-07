#include "TrussDocument/Tokenizer.hpp"
using namespace Truss;

std::vector <Token> Truss::Tokenizer::Scan()
{
    std::vector<Token> result;
    int len = (int)m_Input.size();
    while (m_CurrentIndex < len)
    {
        string word = GetNextWord();
        if (!word.empty())
        {
            if(word == "null") [[unlikely]]
            {
                result.emplace_back(Token{ TokenType::Keyword_Null });
                continue;
            }
            if(word == "true") [[unlikely]]
            {
                result.emplace_back(Token{ TokenType::Keyword_True });
                continue;
            }
            if(word == "false") [[unlikely]]
            {
                result.emplace_back(Token{ TokenType::Keyword_False });
                continue;
            }
            TokenType tokenType = (isdigit(word[0]) ? TokenType::Data_Number : TokenType::Data_Key);
            Token token = { .Type = tokenType, .Data = std::make_shared<string>(std::move(word)) };
            result.emplace_back(std::move(token));
            continue;
        }
        char symbol = GetNextSymbol();
        if (symbol != '\0')
        {
            switch (symbol)
            {
                case ':': result.emplace_back(Token{ TokenType::Symbol_Assignment }); break;
                case '[': result.emplace_back(Token{ TokenType::Symbol_Array_Start }); break;
                case ']': result.emplace_back(Token{ TokenType::Symbol_Array_End }); break;
                case '{': result.emplace_back(Token{ TokenType::Symbol_Object_Start }); break;
                case '}': result.emplace_back(Token{ TokenType::Symbol_Object_End }); break;
                case '(': result.emplace_back(Token{ TokenType::Symbol_Complex_Start }); break;
                case '+': result.emplace_back(Token{ TokenType::Symbol_Add }); break;
                case '-': result.emplace_back(Token{ TokenType::Symbol_Minus }); break;
                case ')': result.emplace_back(Token{ TokenType::Symbol_Complex_End }); break;
                case '"':
                    result.emplace_back(Token{ TokenType::Data_String, std::make_shared<string>(GetNextString()) });
                    break;
                case '#': SkipComment(); continue;
                default: break;
    }
    continue;
}
SkipOneSymbol(); // maybe a space or comma, just skip it (or throw an exception on an unknown character?)
    }
    return result;
}

string Tokenizer::GetNextWord() // TODO: 拆分成 GetNextWord 和 GetNextNumber
{
    string result;
    // if (!std::isalpha(input[index])) return result;
    int index = m_CurrentIndex;
    int len = m_Input.size();
    char ch = m_InputPtr[index];
    bool flag_num = false;
    bool flag_sci = false;
    while (std::isalnum(ch) || ch == '_' || (flag_num && ch == '.') || (flag_sci && ch == '+') || (flag_sci && ch == '-'))
    {
        flag_num = (index + 2 < len && std::isdigit(ch) && std::isdigit(ch+2));
        flag_sci = (ch == 'e' || ch == 'E');
        result.push_back(ch);
        ch = m_InputPtr[++index];
    }
    m_CurrentIndex = index;
    return result;
}

char Tokenizer::GetNextSymbol()
{
    char ch = m_InputPtr[m_CurrentIndex];
    char result = (std::ispunct(ch) ? ch : '\0');
    m_CurrentIndex += (result == '\0' ? 0 : 1);
    return result;
}

void Tokenizer::SkipComment()
{
    int index = m_CurrentIndex;
    char ch = m_InputPtr[index];
    while (ch != '\0' && ch != '\n')
    {
        ch = m_InputPtr[++index];
    }
    m_CurrentIndex = index + (ch == '\0' ? 0 : 1);
}

string Tokenizer::GetNextString()
{
    string result;
    int index = m_CurrentIndex;
    char ch = m_InputPtr[index], ch_next;
    while (ch != '\0' && ch != '\"')
    {
        auto ch_new = { ch };
        if (ch == '\\') [[unlikely]]
        {
            ch_next = m_InputPtr[index + 1];
            if (ch_next == '\\' || ch_next == '"') [[likely]]
            {
                ch_new = { ch_next };
            }
            else [[unlikely]]
            {
                ch_new = {ch, ch_next};
            }
            ++index;
        }
        result.append(ch_new);
        ch = m_InputPtr[++index];
    }
    m_CurrentIndex = index + (ch == '\0' ? 0 : 1);
    return result;
}

void Tokenizer::SkipOneSymbol()
{
    int index = m_CurrentIndex + 1;
    if (index <= m_Input.size())
    {
        m_CurrentIndex = index;
    }
}
