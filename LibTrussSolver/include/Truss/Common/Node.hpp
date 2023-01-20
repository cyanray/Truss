#pragma once
#include "Constants.hpp"

namespace Truss
{
    struct Node
    {
        ID Id = INVALID_ID;
        Numeric X = 0;
        Numeric Y = 0;
        Numeric Z = 0;
    };
}