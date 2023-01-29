#pragma once

#include "Constants.hpp"
#include "IComponent.hpp"

namespace Truss
{
    struct Node : public IComponent
    {
        Numeric X{};
        Numeric Y{};
        Numeric Z{};
    };
}