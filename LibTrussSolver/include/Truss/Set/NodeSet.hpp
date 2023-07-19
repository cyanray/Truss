#pragma once
#include "SetBase.hpp"
#include "Truss/Common/Node.hpp"
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
        [[nodiscard]] vector<ID> GetNodeIds() const override;
    };

}// namespace Truss::Set
