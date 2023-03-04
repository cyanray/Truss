#include "Truss/Common/Constants.hpp"
#include "Truss/Common/Node.hpp"

Truss::Numeric Truss::GetLength(const Truss::Node& node1, const Truss::Node& node2)
{
    return MakeVector<Numeric>(node1, node2).norm();
}
