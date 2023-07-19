#pragma once

#include "ConstraintBase.hpp"
#include "Truss/Common/Common.hpp"
#include "Truss/Common/Resources.hpp"
#include "Truss/Set/NodeSet.hpp"

namespace Truss::Constraint
{
    struct NodeSetConstraint : public ConstraintBase
    {
        bool XDisplacement{false};
        bool YDisplacement{false};
        bool ZDisplacement{false};
        bool XRotation{false};
        bool YRotation{false};
        bool ZRotation{false};
        ID NodeSetKey{INVALID_ID};
        Set::NodeSet* NodeSet{};

        void Build(Resources& resources) override
        {
            auto& set = resources.Sets.at(NodeSetKey);
            NodeSet = std::static_pointer_cast<Set::NodeSet>(set).get();
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override
        {
            return NodeSet->GetNodeIds();
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
