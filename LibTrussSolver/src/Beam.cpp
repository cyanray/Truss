#include "Truss/Element/Beam.hpp"
#include "Truss/Common/Resources.hpp"
#include "Truss/Common/Coordinate.hpp"

namespace Truss::Element
{
    void Beam::Build(Resources& res)
    {
        LeftNode = res.Get<Node>(res.Nodes, LeftNodeKey);
        RightNode = res.Get<Node>(res.Nodes, RightNodeKey);
        Section = res.GetAndCast<Section::Section_Beam>(res.Sections, SectionKey);
        if (YDirectionNodeKey != INVALID_ID)
        {
            YDirectionNode = res.Get<Node>(res.Nodes, YDirectionNodeKey);
        }
    }

    ValidationInfo Beam::Validate() const
    {
        if (LeftNode == nullptr) return {"LeftNode is null"};
        if (RightNode == nullptr) return {"RightNode is null"};
        if (Section == nullptr) return {"Section is null"};
        return {};
    }

    std::vector<ID> Beam::GetNodeIds() const
    {
        return {this->LeftNode->Id, this->RightNode->Id};
    }

    MatrixX Beam::GetStiffnessLocal() const
    {
        Numeric L = GetBeamLength();
        Numeric L2 = L * L;
        Numeric L3 = L2 * L;

        Numeric A = Section->Area;
        Numeric Iy = Section->Iy;
        Numeric Iz = Section->Iz;
        Numeric J = Section->J;
        Numeric E = Section->Mat->YoungsModules;
        Numeric G = Section->Mat->CalculateShearModules();

        Matrix12x12 result = Matrix12x12::Zero();

        Eigen::Matrix<Numeric, 2, 2> local1
        {
            {  1, -1 },
            { -1,  1 }
        };
        result({ 0, 6 }, { 0, 6 }) = (E * A / L) * local1;
        result({ 3, 9 }, { 3, 9 }) = (G * J / L) * local1;

        Eigen::Matrix<Numeric, 4, 4> local2
        {
            {  12,  6*L,  -12,  6*L },
            { 6*L, 4*L2, -6*L, 2*L2 },
            { -12, -6*L,   12, -6*L },
            { 6*L, 2*L2, -6*L, 4*L2 }
        };
        result({ 1, 5, 7, 11 }, { 1, 5, 7, 11 }) = (E * Iz / L3) * local2;
        result({ 2, 4, 8, 10 }, { 2, 4, 8, 10 }) = (E * Iy / L3) * local2;

        return result;
    }

    MatrixX Beam::GetStiffnessGlobal() const
    {
        Vector3 x_axis = MakeVector(*LeftNode, *RightNode);
        auto lambda_matrix = GetTransformationMatrixNature(x_axis);
        auto trans_matrix = BlockDiagonal(lambda_matrix, 2 * GetNodeCount());
        return trans_matrix.transpose() * GetStiffnessLocal() * trans_matrix;
    }

    Numeric Beam::GetBeamLength() const
    {
        return GetLength(*LeftNode, *RightNode);
    }

}// namespace Truss::Element