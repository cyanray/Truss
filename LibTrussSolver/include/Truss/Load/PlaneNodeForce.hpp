#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"
#include "Truss/Common/Resources.hpp"
#include "LoadBase.hpp"


namespace Truss::Load
{
    class PlaneNodeForce : public LoadBase
    {
    public:
        Numeric XForce{};
        Numeric YForce{};
        ID PlaneNodeKey{INVALID_ID};
        PlaneNode* PlaneNode{};

        void Build(Resources& resources) override
        {
            PlaneNode =  &resources.PlaneNodes.at(PlaneNodeKey);
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override
        {
            return { PlaneNode->Id };
        }

        [[nodiscard]] Eigen::Vector<Numeric,6> GetLoad() const override
        {
            return { XForce, YForce, 0, 0, 0, 0 };
        }
    };
}