#pragma once

#include "LoadBase.hpp"
#include "Truss/Common/Common.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Common/Resources.hpp"


namespace Truss::Load
{
    class NodeLoad : public LoadBase
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
            Node = &resources.Nodes.at(NodeKey);
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override
        {
            return {Node->Id};
        }

        [[nodiscard]] std::vector<std::pair<ID, LoadVector>> GetLoads() const override
        {
            LoadVector load = {XForce, YForce, ZForce, XMoment, YMoment, ZMoment};
            return { std::make_pair(Node->Id, load) };
        }
    };
}// namespace Truss::Load