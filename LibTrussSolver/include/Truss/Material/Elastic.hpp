#pragma once

#include "Truss/Common/Constants.hpp"
#include "MaterialBase.hpp"

namespace Truss::Material
{
    struct Elastic : public MaterialBase
    {
        Numeric Density{};
        Numeric YoungsModules{};
        Numeric PoissonRation{};
    };
}