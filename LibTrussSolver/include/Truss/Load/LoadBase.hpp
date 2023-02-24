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
        [[nodiscard]] virtual Eigen::Vector<Numeric, ALL_DOF> GetLoad() const = 0;
    };

}

