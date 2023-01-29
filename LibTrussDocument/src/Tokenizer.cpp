#include <cctype>
#include "TrussDocument/Tokenizer.hpp"

using namespace Truss;
using namespace std;

std::vector<Token> Truss::Tokenizer::Scan()
{
    std::vector<Token> result;
    int len = (int) m_Input.size();
    while (m_CurrentIndex < len)
    {
        ScanWord(result);
        ScanNumber(result);
        ScanSymbol(result);
        SkipSpaceComma();
    }
    return result;
}

void Truss::Tokenizer::ScanWord(std::vector<Token>& result)
{
    string word = GetNextWord();
    if (word.empty()) return;
    if (word == "null") [[unlikely]]
    {
        result.emplace_back(Token{TokenType::Keyword_Null});
        return;
    }
    if (word == "true") [[unlikely]]
    {
        result.emplace_back(Token{TokenType::Keyword_True});
        return;
    }
    if (word == "false") [[unlikely]]
    {
        result.emplace_back(Token{TokenType::Keyword_False});
        return;
    }
    Token token = {.Type = TokenType::Data_Key, .Data = std::make_shared<string>(std::move(word))};
    result.emplace_back(std::move(token));
}

void Tokenizer::ScanNumber(vector<Token> &result)
{
    string number = GetNextNumber();
    if (number.empty()) return;
    Token token = {.Type = TokenType::Data_Number, .Data = std::make_shared<string>(std::move(number))};
    result.emplace_back(std::move(token));
}

void Tokenizer::ScanSymbol(std::vector<Token>& result)
{
    char symbol = GetNextSymbol();
    if (symbol == '\0') return;
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
            result.emplace_back(Token{TokenType::Data_String, std::make_shared<string>(GetNextString())});
            break;
        case '#': SkipComment(); break;
        default: break;
    }
}

string Tokenizer::GetNextWord()
{
    string result;
    if (!std::isalpha(m_InputPtr[m_CurrentIndex])) return result;
    int index = m_CurrentIndex;
    char ch = m_InputPtr[index];
    do
    {
        result.push_back(ch);
        ch = m_InputPtr[++index];
    } while (std::isalnum(ch) || ch == '_');
    m_CurrentIndex = index;
    return result;
}

string Tokenizer::GetNextNumber()
{
    string result;
    if (!std::isdigit(m_InputPtr[m_CurrentIndex])) return result;
    int index = m_CurrentIndex;
    char ch = m_InputPtr[index];
    do
    {
        result.push_back(ch);
        ch = m_InputPtr[++index];
    } while (std::isdigit(ch) || ch == '_' || ch == '.' || ch == '+' || ch == '-' || ch == 'e' || ch == 'E');
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
        auto ch_new = {ch};
        if (ch == '\\') [[unlikely]]
        {
            ch_next = m_InputPtr[index + 1];
            if (ch_next == '\\' || ch_next == '"') [[likely]]
            {
                ch_new = {ch_next};
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

void Tokenizer::SkipSpaceComma()
{
    int index = m_CurrentIndex;
    char ch = m_InputPtr[index];
    while (ch == ' ' || ch == ',' || ch == '\t' || ch == '\n' || ch == '\r')
    {
        ch = m_InputPtr[++index];
    }
    m_CurrentIndex = index;
}


