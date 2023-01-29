#pragma once

#include "Truss/Common/IComponent.hpp"

namespace Truss::Element
{

    class ElementBase : public IComponent
    {
    public:
        virtual void Build(Resources&) = 0;

        [[nodiscard]] virtual std::vector<ID> GetNodeIds() const = 0;

        [[nodiscard]] virtual MatrixX<Numeric> GetStiffnessGlobal() const = 0;

    };

}