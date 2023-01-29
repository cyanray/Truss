#include "Truss/Element/PlaneBar.hpp"
#include "Truss/Common/Resources.hpp"
using namespace Truss;

Numeric Element::PlaneBar::GetBarLength() const
{
    return static_cast<Numeric>(sqrt(pow(RightNode->X - LeftNode->X, 2) + pow(RightNode->Y - LeftNode->Y, 2)));
}

constexpr int Element::PlaneBar::GetStiffnessSize() const noexcept
{
    return 4;
}

Matrix4x4<Numeric> Element::PlaneBar::GetStiffnessLocal() const
{
    auto E = Section->Mat->YoungsModules;
    Numeric bar_length = GetBarLength();
    Numeric A = Section->Area;
    Matrix4x4<Numeric> result
            {
                    {1,  0, -1, 0},
                    {0,  0, 0,  0},
                    {-1, 0, 1,  0},
                    {0,  0, 0,  0}
            };
    return (E * A / bar_length) * result;
}

Matrix4x4<Numeric> Element::PlaneBar::_GetStiffnessGlobal() const
{
    Numeric angle_rad = atan((RightNode->Y - LeftNode->Y) / (RightNode->X - LeftNode->X));
    auto transformation_matrix = GetCoordinateTransformationMatrix<Numeric>(angle_rad);
    return transformation_matrix.transpose() * GetStiffnessLocal() * transformation_matrix;
}

void Element::PlaneBar::Build(Resources& resources)
{
    this->LeftNode = &resources.PlaneNodes.at(LeftNodeKey);
    this->RightNode = &resources.PlaneNodes.at(RightNodeKey);
    this->Section = &resources.Sections.at(SectionKey);
}

MatrixX<Numeric> Element::PlaneBar::GetStiffnessGlobal() const
{
    Numeric angle_rad = atan((RightNode->Y - LeftNode->Y) / (RightNode->X - LeftNode->X));
    auto transformation_matrix = GetCoordinateTransformationMatrix<Numeric>(angle_rad);
    return transformation_matrix.transpose() * GetStiffnessLocal() * transformation_matrix;
}

std::vector<ID> Element::PlaneBar::GetNodeIds() const
{
    return { this->LeftNode->Id, this->RightNode->Id };
}
