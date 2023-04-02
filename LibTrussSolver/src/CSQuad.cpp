#include "Truss/Element/CSQuad.hpp"
#include "Truss/Common/Resources.hpp"
#include "Truss/Utils/GaussianQuadrature.hpp"

namespace Truss::Element
{
    void CSQuad::Build(Resources& resources)
    {
        Node1 = &resources.Nodes.at(Node1Key);
        Node2 = &resources.Nodes.at(Node2Key);
        Node3 = &resources.Nodes.at(Node3Key);
        Node4 = &resources.Nodes.at(Node4Key);
        auto& section = resources.Sections.at(SectionKey);
        Section = std::static_pointer_cast<Section::Section_CSQuad>(section).get();
    }

    std::vector<ID> CSQuad::GetNodeIds() const
    {
        return {Node1->Id, Node2->Id, Node3->Id, Node4->Id};
    }

    MatrixX<Numeric> CSQuad::GetStiffnessGlobal() const
    {
        return {};
    }

    StressVector CSQuad::CalculateStress(const VectorX<Numeric>& displacement) const
    {
        return {};
    }

    MatrixX<Numeric> CSQuad::GetStiffnessLocal() const
    {
        // TODO: integrate B^TScalar D B
        return {};
    }

    Eigen::Matrix<Numeric, 3, 24> CSQuad::GetBMatrix() const
    {
        // TODO: is a function about x,y
        return {};
    }

    Matrix3x3<Numeric> CSQuad::GetDMatrix() const
    {
        Numeric v = Section->Mat->PoissonRation;
        Numeric E = Section->Mat->YoungsModules;
        if (!IsConstantStrainTriangle) [[likely]]
        {
            Matrix3x3<Numeric> DMatrix{{1, v, 0},
                                       {v, 1, 0},
                                       {0, 0, (1 - v) / 2}};
            return (E / (1 - v * v)) * DMatrix;
        }
        else [[unlikely]]
        {
            Matrix3x3<Numeric> DMatrix{{1 - v, v, 0},
                                       {v, 1 - v, 0},
                                       {0, 0, (1 - 2 * v) / 2}};
            return (E / ((1 + v) * (1 - 2 * v))) * DMatrix;
        }
    }
}