#pragma once

#include "Truss/Common/IComponent.hpp"

namespace Truss::Section
{

    class SectionBase : public IComponent
    {
    public:
        virtual void Build(Resources&) = 0;
    };

}// namespace Truss::Section
