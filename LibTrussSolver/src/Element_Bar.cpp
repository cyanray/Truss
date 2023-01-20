#include "Truss/Element/Bar.hpp"

using namespace Truss;

Numeric Element::Bar::GetBarLength() const
{
    return static_cast<Numeric>(sqrt(pow(RightNode->X - LeftNode->X, 2) + pow(RightNode->Y - LeftNode->Y, 2)));
}

constexpr int Element::Bar::GetStiffnessSize() const noexcept
{
    return 4;
}

Matrix4x4<Numeric> Element::Bar::GetStiffnessLocal() const
{
    auto E = Mat->YoungsModules;
    Numeric bar_length = GetBarLength();
    Numeric A = Area;
    Matrix4x4<Numeric> result
            {
                    {1,  0, -1, 0},
                    {0,  0, 0,  0},
                    {-1, 0, 1,  0},
                    {0,  0, 0,  0}
            };
    return (E * A / bar_length) * result;
}

Matrix4x4<Numeric> Element::Bar::GetStiffnessGlobal() const
{
    Numeric angle_rad = atan((RightNode->Y - LeftNode->Y) / (RightNode->X - LeftNode->X));
    auto transformation_matrix = GetCoordinateTransformationMatrix<Numeric>(angle_rad);
    return transformation_matrix.transpose() * GetStiffnessLocal() * transformation_matrix;
}
