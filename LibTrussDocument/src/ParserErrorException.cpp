#include "TrussDocument/ParserErrorException.hpp"

namespace Truss
{
    UnexpectedTokenException::UnexpectedTokenException(TokenType expectation, TokenType actual)
    {
        message = "Unexpected token: ";
        message += GetTokenTypeName(actual);
        message += ", expected: ";
        message += GetTokenTypeName(expectation);
    }

    UnexpectedTokenException::UnexpectedTokenException(TokenType expectation)
    {
        message = "Expecting token: ";
        message += GetTokenTypeName(expectation);
        message += ", but got nothing.";
    }
    UnexpectedTokenException::UnexpectedTokenException()
    {
        message = "Expecting value token, but got nothing.";
    }
    const char* UnexpectedTokenException::what() const noexcept
    {
        return message.data();
    }

    ParseNumberException::ParseNumberException(std::string_view value)
    {
        message = "Cannot parse: ";
        message += value;
        message += " as a number.";
    }
    const char* ParseNumberException::what() const noexcept
    {
        return message.data();
    }
}