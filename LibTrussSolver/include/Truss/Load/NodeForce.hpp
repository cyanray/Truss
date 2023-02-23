#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Common/Resources.hpp"
#include "LoadBase.hpp"


namespace Truss::Load
{
    class NodeForce : public LoadBase
    {
    public:
        Numeric XForce{};
        Numeric YForce{};
        Numeric ZForce{};
        ID NodeKey{INVALID_ID};
        Node* Node{};

        void Build(Resources& resources) override
        {
            Node =  &resources.Nodes.at(NodeKey);
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override
        {
            return { Node->Id };
        }

        [[nodiscard]] Eigen::Vector<Numeric, MAX_DOF> GetLoad() const override
        {
            return { XForce, YForce, ZForce };
        }
    };
}