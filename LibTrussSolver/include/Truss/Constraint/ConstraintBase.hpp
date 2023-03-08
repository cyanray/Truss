#pragma once

#include "Truss/Common/IComponent.hpp"
#include "Truss/Common/DegreeOfFreedom.hpp"
#include <vector>
namespace Truss::Constraint
{

    class ConstraintBase : public IComponent
    {
    public:
        virtual void Build(Resources&) = 0;
        [[nodiscard]] virtual std::vector<ID> GetNodeIds() const = 0;
        [[nodiscard]] virtual std::vector<bool> GetConstraint() const = 0;
        [[nodiscard]] virtual DegreeOfFreedom GetDegreeOfFreedom() const = 0;
    };

}
