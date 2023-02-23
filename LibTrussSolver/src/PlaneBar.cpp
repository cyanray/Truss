#include "Truss/Element/PlaneBar.hpp"
#include "Truss/Common/Resources.hpp"
using namespace Truss;

Numeric Element::PlaneBar::GetBarLength() const
{
    return static_cast<Numeric>(sqrt(pow(RightNode->X - LeftNode->X, 2) + pow(RightNode->Y - LeftNode->Y, 2)));
}


MatrixX<Numeric> Element::PlaneBar::GetStiffnessLocal() const
{
    auto E = Section->Mat->YoungsModules;
    Numeric bar_length = GetBarLength();
    Numeric A = Section->Area;
    Eigen::Matrix<Numeric, 2, 2> local
    {
        {  1, -1 },
        { -1,  1 }
    };
    Matrix12x12<Numeric> result = Matrix12x12<Numeric>::Zero();
    result({ 0, 6 }, { 0, 6 }) = local;
    return (E * A / bar_length) * result;
}

void Element::PlaneBar::Build(Resources& resources)
{
    this->LeftNode = &resources.PlaneNodes.at(LeftNodeKey);
    this->RightNode = &resources.PlaneNodes.at(RightNodeKey);
    this->Section = std::static_pointer_cast<Section::Section_PlaneBar>(resources.Sections.at(SectionKey)).get();
}

MatrixX<Numeric> Element::PlaneBar::GetStiffnessGlobal() const
{
    Vector3<Numeric> x_asix{ LeftNode->X - RightNode->X, LeftNode->Y - RightNode->Y, 0 };
    auto lambda_matrix = GetTransformationMatrix<Numeric>(x_asix);
    auto trans_matrix = BlockDiagonal<Numeric>(lambda_matrix, 2 * GetNodeCount());
    return trans_matrix.transpose() * GetStiffnessLocal() * trans_matrix;   // 12x12 Matrix
}

std::vector<ID> Element::PlaneBar::GetNodeIds() const
{
    return { this->LeftNode->Id, this->RightNode->Id };
}

