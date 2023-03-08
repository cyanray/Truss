#pragma once
#include "SectionBase.hpp"
#include "Truss/Material/Elastic.hpp"


namespace Truss::Section
{
    class Section_Beam : public SectionBase
    {
    public:
        ID MaterialKey{INVALID_ID};
        Numeric Area{};
        Numeric Iy{};
        Numeric Iz{};
        Numeric J{};
        Material::Elastic* Mat{};

        void Build(Resources& resources) override;
    };
}// namespace Truss::Section
