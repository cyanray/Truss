#pragma once
#include <string>

namespace Truss
{
    class ValidationInfo
    {
    public:
        bool IsValid{true};
        std::string Message{};

        ValidationInfo() = default;
        ValidationInfo(std::string message)
            : IsValid(false), Message(std::move(message))
        {
        }
    };

}