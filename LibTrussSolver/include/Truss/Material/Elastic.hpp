#pragma once

#include "MaterialBase.hpp"
#include "Truss/Common/Constants.hpp"

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
}// namespace Truss::Material