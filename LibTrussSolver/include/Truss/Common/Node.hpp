#pragma once

#include "Constants.hpp"
#include "IComponent.hpp"

namespace Truss
{
    struct Node : public IComponent
    {
        Numeric X{};
        Numeric Y{};
        Numeric Z{};
    };

    template<typename T>
    Vector3<T> MakeVector(const Node& node1, const Node& node2)
    {
        return {node2.X - node1.X, node2.Y - node1.Y, node2.Z - node1.Z};
    }

    /**
     * @brief Get the length between two nodes.
     * @param node1 Node 1.
     * @param node2 Node 2.
     * @return Length between node1 and node2.
     */
    Numeric GetLength(const Node& node1, const Node& node2);
}// namespace Truss