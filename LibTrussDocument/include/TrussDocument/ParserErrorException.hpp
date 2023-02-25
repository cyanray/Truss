#pragma once
#include "TrussDocument/Tokenizer.hpp"
#include <stdexcept>
#include <string>
using std::string;

namespace Truss
{

    class UnexpectedTokenException : public std::exception
    {
    public:
        UnexpectedTokenException(TokenType expectation, TokenType actual);
        explicit UnexpectedTokenException(TokenType expectation);
        UnexpectedTokenException();

        [[nodiscard]] const char* what() const override;

        ~UnexpectedTokenException() noexcept override = default;
    private:
        std::string message;
    };

    class ParseNumberException : public std::exception
    {
    public:
        explicit ParseNumberException(std::string_view value);

        [[nodiscard]] const char* what() const override;

        ~ParseNumberException() noexcept override = default;
    private:
        std::string message;
    };

}
