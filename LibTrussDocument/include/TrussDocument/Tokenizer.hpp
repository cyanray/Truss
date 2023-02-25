#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <memory>
using std::shared_ptr;
using std::string;
using std::vector;

namespace Truss
{
	enum class TokenType
	{
		Data_Key,
		Data_Number,			// including type suffixes
        Data_String,
		Keyword_Null,			// null
        Keyword_True,           // true
        Keyword_False,          // false
		Symbol_Assignment,		// symbol :
		Symbol_Object_Start,	// symbol {
		Symbol_Object_End,		// symbol }
		Symbol_Array_Start,		// symbol [
		Symbol_Array_End,		// symbol ]
		Symbol_Complex_Start,	// symbol (
		Symbol_Complex_End,		// symbol )
        Symbol_Add,     		// symbol +
        Symbol_Minus            // symbol -
	};

    std::string_view GetTokenTypeName(TokenType type);

	struct Token
	{
		TokenType Type;
		shared_ptr<string> Data;
	};

	class Tokenizer
	{
	public:

		explicit Tokenizer(std::string_view input):m_Input(input), m_CurrentIndex(0), m_InputPtr(input.data())
		{
		}

		std::vector<Token> Scan();
	private:
		int m_CurrentIndex;
        std::string_view m_Input;
        const char* m_InputPtr;

		string GetNextWord();
        string GetNextNumber();
		char GetNextSymbol();
        string GetNextString();
		void SkipComment();
        void SkipSpaceComma();

        void ScanWord(std::vector<Token>& result);
        void ScanNumber(std::vector<Token>& result);
        void ScanSymbol(std::vector<Token>& result);

	};

}