#include "Truss/Element/Beam.hpp"
#include "Truss/Common/Resources.hpp"

namespace Truss::Element
{
    void Beam::Build(Resources& resources)
    {
        this->LeftNode = &resources.Nodes.at(LeftNodeKey);
        this->RightNode = &resources.Nodes.at(RightNodeKey);
        auto& section = resources.Sections.at(SectionKey);
        this->Section = std::static_pointer_cast<Section::Section_Beam>(section).get();
        if (YDirectionNodeKey != INVALID_ID)
        {
            this->YDirectionNode = &resources.Nodes.at(YDirectionNodeKey);
        }
    }

    std::vector<ID> Beam::GetNodeIds() const
    {
        return {this->LeftNode->Id, this->RightNode->Id};
    }

    MatrixX<Numeric> Beam::GetStiffnessLocal() const
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

        Matrix12x12<Numeric> result = Matrix12x12<Numeric>::Zero();

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

    MatrixX<Numeric> Beam::GetStiffnessGlobal() const
    {
        Vector3<Numeric> x_axis = MakeVector<Numeric>(*LeftNode, *RightNode);
        auto lambda_matrix = GetTransformationMatrixAuto<Numeric>(x_axis);
        auto trans_matrix = BlockDiagonal<Numeric>(lambda_matrix, 2 * GetNodeCount());
        return trans_matrix.transpose() * GetStiffnessLocal() * trans_matrix;
    }

    Numeric Beam::GetBeamLength() const
    {
        return GetLength(*LeftNode, *RightNode);
    }

}// namespace Truss::Element