#pragma once

#include "Constants.hpp"
#include "IComponent.hpp"

namespace Truss
{
    struct PlaneNode : public IComponent
    {
        Numeric X{};
        Numeric Y{};
    };
}
