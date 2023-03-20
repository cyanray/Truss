#include "Truss/Common/Resources.hpp"
#include "Truss/Element/CSTriangle.hpp"
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
        assert(area > 0);
        return area;
    }

    std::vector<ID> CSTriangle::GetNodeIds() const
    {
        return {LeftNode->Id, RightNode->Id, TopNode->Id};
    }

    [[nodiscard]] MatrixX<Numeric> CSTriangle::GetStiffnessLocal() const
    {
        Numeric A = GetTriangleArea();
        Numeric t = Section->Thickness;
        Numeric E = Section->Mat->YoungsModules;
        Numeric v = Section->Mat->PoissonRation;
        Matrix3x3<Numeric> DMatrix{{1, v, 0},
                                   {v, 1, 0},
                                   {0, 0, (1 - v) / 2}};

        Numeric xi = LeftNode->X, xj = RightNode->X, xk = TopNode->X;
        Numeric yi = LeftNode->Y, yj = RightNode->Y, yk = TopNode->Y;
        Numeric bi = yj - yk, bj = yk - yi, bk = yi - yj;
        Numeric gi = xk - xj, gj = xi - xk, gk = xj - xi;
        Eigen::Matrix<Numeric, 3, 18> BMatrix{{bi, 0, 0, 0, 0, 0, bj, 0, 0, 0, 0, 0, bk, 0, 0, 0, 0, 0},
                                              {0, gi, 0, 0, 0, 0, 0, gj, 0, 0, 0, 0, 0, gk, 0, 0, 0, 0},
                                              {gi, bi, 0, 0, 0, 0, gj, bj, 0, 0, 0, 0, gk, bk, 0, 0, 0, 0}};

        Numeric k = (t * E) / (4 * A * (1 - v * v));
        return k * BMatrix.transpose() * DMatrix * BMatrix;
    }

    MatrixX<Numeric> CSTriangle::GetStiffnessGlobal() const
    {
        return GetStiffnessLocal();
    }
}// namespace Truss::Element
