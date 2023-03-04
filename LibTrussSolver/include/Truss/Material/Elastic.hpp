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

        [[nodiscard]] Numeric CalculateShearModules() const
        {
            return YoungsModules / (2 * (1 + PoissonRation));
        }
    };
}