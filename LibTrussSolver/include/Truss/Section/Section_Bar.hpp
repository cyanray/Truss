#pragma once

#include "SectionBase.hpp"
#include "Truss/Common/Common.hpp"
#include "Truss/Material/Elastic.hpp"

namespace Truss::Section
{
    struct Section_Bar : public SectionBase
    {
        Numeric Area{};
        ID MaterialKey{INVALID_ID};
        Material::Elastic* Mat{};

        void Build(Resources& resources) override;
    };
}// namespace Truss::Section