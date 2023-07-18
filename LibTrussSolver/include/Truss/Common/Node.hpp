#pragma once

#include "Common.hpp"
#include "IComponent.hpp"

namespace Truss
{
    struct Node : public IComponent
    {
        Numeric X{};
        Numeric Y{};
        Numeric Z{};
    };

    inline Vector3 ToVector(const Node& node)
    {
        return {node.X, node.Y, node.Z};
    }

    inline Vector3 MakeVector(const Node& node1, const Node& node2)
    {
        return {node2.X - node1.X, node2.Y - node1.Y, node2.Z - node1.Z};
    }

    /**
     * @brief Get the length between two nodes.
     * @param node1 Node 1.
     * @param node2 Node 2.
     * @return Length between node1 and node2.
     */
    inline Numeric GetLength(const Node& node1, const Node& node2)
    {
        return MakeVector(node1, node2).norm();
    }
}// namespace Truss