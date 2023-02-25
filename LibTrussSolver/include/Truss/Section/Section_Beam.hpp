#pragma once
#include "SectionBase.hpp"
#include "Truss/Material/Elastic.hpp"


namespace Truss::Section
{
    class Section_Beam : public SectionBase
    {
        ID MaterialKey{INVALID_ID};
        Material::Elastic* Mat{};

        void Build(Resources& resources) override;
    };
}
