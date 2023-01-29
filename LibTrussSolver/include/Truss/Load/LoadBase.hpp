#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Common/IComponent.hpp"
#include <vector>
namespace Truss::Load
{

    class LoadBase : public IComponent
    {
    public:
        virtual void Build(Resources&) = 0;
        [[nodiscard]] virtual std::vector<ID> GetNodeIds() const = 0;
        [[nodiscard]] virtual std::vector<Numeric> GetLoad() const = 0;
    };

}

