#pragma once

#include "Common.hpp"
#include <string>
using std::string;
namespace Truss
{

    class IComponent
    {
    public:
        ID Key{INVALID_ID};
        ID Id{INVALID_ID};
        virtual ~IComponent() = default;
    };

}// namespace Truss
