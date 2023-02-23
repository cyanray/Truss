#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Common/Node.hpp"
#include "ConstraintBase.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Constraint
{
    struct NodeDisplacement : public ConstraintBase
    {
        bool XConstraint{false};
        bool YConstraint{false};
        bool ZConstraint{false};
        ID NodeKey{INVALID_ID};
        Node* Node{};

        void Build(Resources& resources) override
        {
            Node = &resources.Nodes.at(NodeKey);
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override
        {
            return { Node->Id };
        }

        [[nodiscard]] std::vector<bool> GetConstraint() const override
        {
            return { XConstraint, YConstraint, ZConstraint };
        }
    };
}