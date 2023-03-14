#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Common/IComponent.hpp"
#include <Eigen/Dense>
namespace Truss::Load
{

    class LoadBase : public IComponent
    {
    public:
        virtual void Build(Resources&) = 0;
        [[nodiscard]] virtual std::vector<ID> GetNodeIds() const = 0;
        [[nodiscard]] virtual std::vector<std::pair<ID, LoadVector>> GetLoads() const = 0;
    };

}// namespace Truss::Load
