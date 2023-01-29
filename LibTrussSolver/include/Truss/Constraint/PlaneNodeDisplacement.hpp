#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"
#include "ConstraintBase.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Constraint
{
    struct PlaneNodeDisplacement : public ConstraintBase
    {
        bool XConstraint{false};
        bool YConstraint{false};
        ID PlaneNodeKey{INVALID_ID};
        PlaneNode* PlaneNode{};

        void Build(Resources& resources) override
        {
            PlaneNode = &resources.PlaneNodes.at(PlaneNodeKey);
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override
        {
            return { PlaneNode->Id };
        }

        [[nodiscard]] std::vector<bool> GetConstraint() const override
        {
            return { XConstraint, YConstraint };
        }
    };
}