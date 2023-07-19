#include "NodeSet.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Set
{
    void NodeSet::Build(Resources& resources)
    {
        Nodes.reserve(NodeKeys.size());
        for (auto& key : NodeKeys)
        {
            Nodes.emplace_back(&resources.Nodes.at(key));
        }
    }
    vector<ID> NodeSet::GetNodeIds() const
    {
        vector<ID> result;
        for(Node* node : Nodes)
        {
            result.push_back(node->Id);
        }
        return result;
    }
}// namespace Truss::Set