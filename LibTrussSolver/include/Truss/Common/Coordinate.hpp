#pragma once
#include "Common.hpp"
#include "Node.hpp"

namespace Truss
{
    Matrix3x3 GetTransformationMatrix(const Vector3& XAxis, const Vector3& YAxis = Vector3::Zero());

    Matrix3x3 GetTransformationMatrixNature(const Vector3& XAxis);

    Matrix3x3 GetTransformationMatrixNature(const Node& origin, const Node& node1, const Node& node2);
}