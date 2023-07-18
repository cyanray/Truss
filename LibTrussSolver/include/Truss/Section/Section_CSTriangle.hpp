#pragma once
#include "SectionBase.hpp"
#include "Truss/Material/Elastic.hpp"

namespace Truss::Section
{
    class Section_CSTriangle : public SectionBase
    {
    public:
        ID MaterialKey{INVALID_ID};
        bool IsConstantStrainTriangle{false};
        Numeric Thickness{};
        Material::Elastic* Mat{};

        void Build(Resources& resources) override;

    };

}// namespace Truss::Section
