#pragma once
#include "Truss/Common/Node.hpp"
#include "SetBase.hpp"
#include <vector>
using std::vector;

namespace Truss::Set
{
    class NodeSet : public SetBase
    {
    public:
        vector<ID> NodeKeys{};
        vector<Node*> Nodes{};
        void Build(Resources& resources) override;
    };

}// namespace Truss::Set
