#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Material/Elastic.hpp"
#include "SectionBase.hpp"

namespace Truss::Section
{
    struct Section_Bar : public SectionBase
    {
        Numeric Area{};
        ID MaterialKey{INVALID_ID};
        Material::Elastic* Mat{};

        void Build(Resources& resources) override;
    };
}