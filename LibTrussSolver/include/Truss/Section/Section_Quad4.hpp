#pragma once
#include "SectionBase.hpp"
#include "Truss/Material/Elastic.hpp"


namespace Truss::Section
{
    class Section_Quad4 : public SectionBase
    {
    public:
        ID MaterialKey{INVALID_ID};
        Numeric Thickness{};
        Material::Elastic* Mat{};

        void Build(Resources& resources) override;
    };
}// namespace Truss::Section