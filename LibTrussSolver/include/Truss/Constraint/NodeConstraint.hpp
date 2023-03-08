#pragma once

#include "ConstraintBase.hpp"
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Constraint
{
    struct NodeConstraint : public ConstraintBase
    {
        bool XDisplacement{false};
        bool YDisplacement{false};
        bool ZDisplacement{false};
        bool XRotation{false};
        bool YRotation{false};
        bool ZRotation{false};
        ID NodeKey{INVALID_ID};
        Truss::Node* Node{};

        void Build(Resources& resources) override
        {
            Node = &resources.Nodes.at(NodeKey);
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override
        {
            return {Node->Id};
        }

        [[nodiscard]] std::vector<bool> GetConstraint() const override
        {
            return {XDisplacement, YDisplacement, ZDisplacement, XRotation, YRotation, ZRotation};
        }

        [[nodiscard]] DegreeOfFreedom GetDegreeOfFreedom() const override
        {
            DofBitField dof(!XDisplacement, !YDisplacement, !ZDisplacement, !XRotation, !YRotation, !ZRotation);
            return *reinterpret_cast<DegreeOfFreedom*>(&dof);
        }
    };
}// namespace Truss::Constraint