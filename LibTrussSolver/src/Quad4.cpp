#include "Truss/Element/Quad4.hpp"
#include "Truss/Common/Resources.hpp"
#include "Truss/Utils/GaussianQuadrature.hpp"

namespace Truss::Element
{
    void Quad4::Build(Resources& resources)
    {
        Node1 = &resources.Nodes.at(Node1Key);
        Node2 = &resources.Nodes.at(Node2Key);
        Node3 = &resources.Nodes.at(Node3Key);
        Node4 = &resources.Nodes.at(Node4Key);
        auto& section = resources.Sections.at(SectionKey);
        Section = std::static_pointer_cast<Section::Section_Quad4>(section).get();
    }

    ValidationInfo Quad4::Validate() const
    {
        if (Node1 == nullptr) return {"Node1 is null"};
        if (Node2 == nullptr) return {"Node2 is null"};
        if (Node3 == nullptr) return {"Node3 is null"};
        if (Node4 == nullptr) return {"Node4 is null"};
        if (Section == nullptr) return {"Section is null"};
        return {};
    }

    std::vector<ID> Quad4::GetNodeIds() const
    {
        return {Node1->Id, Node2->Id, Node3->Id, Node4->Id};
    }

    MatrixX Quad4::GetStiffnessGlobal() const
    {
        return {};
    }

    StressVector Quad4::CalculateStress(const VectorX& displacement) const
    {
        return {};
    }

    MatrixX Quad4::GetStiffnessLocal() const
    {
        // TODO: integrate B^TScalar D B
        return {};
    }

    Eigen::Matrix<Numeric, 3, 24> Quad4::GetBMatrix(Numeric s, Numeric t) const
    {
        Eigen::Matrix<Numeric, 3, 24> result = Eigen::Matrix<Numeric, 3, 24>::Zero();
        Eigen::Vector<Numeric, 4> x {Node1->X, Node2->X, Node3->X, Node4->X};
        Eigen::Vector<Numeric, 4> y {Node1->Y, Node2->Y, Node3->Y, Node4->Y};

        return result;
    }

    Matrix3x3 Quad4::GetDMatrix() const
    {
        Numeric v = Section->Mat->PoissonRation;
        Numeric E = Section->Mat->YoungsModules;
        if (!IsConstantStrainTriangle) [[likely]]
        {
            Matrix3x3 DMatrix{{1, v, 0},
                                       {v, 1, 0},
                                       {0, 0, (1 - v) / 2}};
            return (E / (1 - v * v)) * DMatrix;
        }
        else [[unlikely]]
        {
            Matrix3x3 DMatrix{{1 - v, v, 0},
                                       {v, 1 - v, 0},
                                       {0, 0, (1 - 2 * v) / 2}};
            return (E / ((1 + v) * (1 - 2 * v))) * DMatrix;
        }
    }
}