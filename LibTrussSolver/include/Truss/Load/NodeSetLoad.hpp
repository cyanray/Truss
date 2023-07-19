#pragma once

#include "LoadBase.hpp"
#include "Truss/Common/Common.hpp"
#include "Truss/Common/Resources.hpp"
#include "Truss/Set/NodeSet.hpp"

namespace Truss::Load
{
    class NodeSetLoad : public LoadBase
    {
    public:
        Numeric XForce{};
        Numeric YForce{};
        Numeric ZForce{};
        Numeric XMoment{};
        Numeric YMoment{};
        Numeric ZMoment{};
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

        [[nodiscard]] std::vector<std::pair<ID, LoadVector>> GetLoads() const override
        {
            LoadVector load = {XForce, YForce, ZForce, XMoment, YMoment, ZMoment};
            std::vector<std::pair<ID, LoadVector>> loads;
            for (auto& nodeId : NodeSet->GetNodeIds())
            {
                loads.emplace_back(nodeId, load);
            }
            return loads;
        }

    };

}// namespace Truss::Load
