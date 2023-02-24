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
        Numeric XMoment{};
        Numeric YMoment{};
        Numeric ZMoment{};
        ID NodeKey{INVALID_ID};
        Truss::Node* Node{};

        void Build(Resources& resources) override
        {
            Node =  &resources.Nodes.at(NodeKey);
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override
        {
            return { Node->Id };
        }

        [[nodiscard]] Eigen::Vector<Numeric,6> GetLoad() const override
        {
            return { XForce, YForce, ZForce, XMoment, YMoment, ZMoment };
        }
    };
}