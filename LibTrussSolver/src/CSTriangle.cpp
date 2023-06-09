#include "Truss/Element/CSTriangle.hpp"
#include "Truss/Common/Resources.hpp"
#include <cassert>
#include <iostream>
using namespace Truss;

namespace Truss::Element
{
    void CSTriangle::Build(Resources& resources)
    {
        LeftNode = &resources.Nodes[LeftNodeKey];
        RightNode = &resources.Nodes[RightNodeKey];
        TopNode = &resources.Nodes[TopNodeKey];
        auto& section = resources.Sections.at(SectionKey);
        this->Section = std::static_pointer_cast<Section::Section_CSTriangle>(section).get();
    }

    Numeric CSTriangle::GetTriangleArea() const
    {
        Numeric area = abs(static_cast<Numeric>(0.5) *
                           (LeftNode->X * (RightNode->Y - TopNode->Y) +
                            RightNode->X * (TopNode->Y - LeftNode->Y) +
                            TopNode->X * (LeftNode->Y - RightNode->Y)));
        return area;
    }

    std::vector<ID> CSTriangle::GetNodeIds() const
    {
        return {LeftNode->Id, RightNode->Id, TopNode->Id};
    }

    [[nodiscard]] MatrixX<Numeric> CSTriangle::GetStiffnessLocal() const
    {
        auto DMatrix = GetDMatrix();
        auto BMatrix = GetBMatrix();

        Numeric A = GetTriangleArea();
        Numeric t = Section->Thickness;

        Numeric k = t / (4 * A);
        return k * BMatrix.transpose() * DMatrix * BMatrix;
    }

    MatrixX<Numeric> CSTriangle::GetStiffnessGlobal() const
    {
        return GetStiffnessLocal();
    }

    Matrix3x3<Numeric> CSTriangle::GetDMatrix() const
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

    Eigen::Matrix<Numeric, 3, 18> CSTriangle::GetBMatrix() const
    {
        Numeric xi = LeftNode->X, xj = RightNode->X, xk = TopNode->X;
        Numeric yi = LeftNode->Y, yj = RightNode->Y, yk = TopNode->Y;
        Numeric bi = yj - yk, bj = yk - yi, bk = yi - yj;
        Numeric gi = xk - xj, gj = xi - xk, gk = xj - xi;
        Eigen::Matrix<Numeric, 3, 18> BMatrix{{bi, 0, 0, 0, 0, 0, bj, 0, 0, 0, 0, 0, bk, 0, 0, 0, 0, 0},
                                              {0, gi, 0, 0, 0, 0, 0, gj, 0, 0, 0, 0, 0, gk, 0, 0, 0, 0},
                                              {gi, bi, 0, 0, 0, 0, gj, bj, 0, 0, 0, 0, gk, bk, 0, 0, 0, 0}};
        return BMatrix;
    }

    StressVector CSTriangle::CalculateStress(const VectorX<Numeric>& displacement) const
    {
        assert(displacement.size() == 18);
        StressVector result = StressVector::Zero();
        result({0, 1, 3}) = GetDMatrix() * GetBMatrix() * displacement;
        return result;
    }
}// namespace Truss::Element
