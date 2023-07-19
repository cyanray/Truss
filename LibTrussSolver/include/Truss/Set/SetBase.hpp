#pragma once
#include "Truss/Common/IComponent.hpp"

namespace Truss::Set
{
    class SetBase : public IComponent
    {
    public:
        virtual void Build(Resources&) = 0;

        [[nodiscard]] virtual std::vector<ID> GetNodeIds() const = 0;
    };
}// namespace Truss::Set
