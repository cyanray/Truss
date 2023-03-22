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
}// namespace Truss::Set