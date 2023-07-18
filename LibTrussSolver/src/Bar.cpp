#include "Truss/Element/Bar.hpp"
#include "Truss/Common/Resources.hpp"
#include "Truss/Common/Coordinate.hpp"
using namespace Truss;

void Element::Bar::Build(Resources& res)
{
    LeftNode = res.Get<Node>(res.Nodes, LeftNodeKey);
    RightNode = res.Get<Node>(res.Nodes, RightNodeKey);
    Section = res.GetAndCast<Section::Section_Bar>(res.Sections, SectionKey);
}

ValidationInfo Element::Bar::Validate() const
{
    if (LeftNode == nullptr) return {"LeftNode is null"};
    if (RightNode == nullptr) return {"RightNode is null"};
    if (Section == nullptr) return {"Section is null"};
    return {};
}

Numeric Element::Bar::GetBarLength() const
{
    return GetLength(*LeftNode, *RightNode);
}

MatrixX Element::Bar::GetStiffnessLocal() const
{
    auto E = Section->Mat->YoungsModules;
    Numeric bar_length = GetBarLength();
    Numeric A = Section->Area;
    Eigen::Matrix<Numeric, 2, 2> local
    {
        {  1, -1 },
        { -1,  1 }
    };
    Matrix12x12 result = Matrix12x12::Zero();
    result({0, 6}, {0, 6}) = local;
    return (E * A / bar_length) * result;
}

MatrixX Element::Bar::GetStiffnessGlobal() const
{
    Vector3 x_axis = MakeVector(*LeftNode, *RightNode);
    auto lambda_matrix = GetTransformationMatrix(x_axis);
    auto trans_matrix = BlockDiagonal(lambda_matrix, 2 * GetNodeCount());
    return trans_matrix.transpose() * GetStiffnessLocal() * trans_matrix;
}

std::vector<ID> Element::Bar::GetNodeIds() const
{
    return {this->LeftNode->Id, this->RightNode->Id};
}
