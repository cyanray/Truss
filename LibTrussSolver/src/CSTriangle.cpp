#include "Truss/Element/CSTriangle.hpp"
#include "Truss/Common/Coordinate.hpp"
#include "Truss/Common/Resources.hpp"
#include <cassert>
#include <iostream>
#include <mutex>
using namespace Truss;

namespace Truss::Element
{
    void CSTriangle::Build(Resources& res)
    {
        LeftNode = res.Get<Node>(res.Nodes, LeftNodeKey);
        RightNode = res.Get<Node>(res.Nodes, RightNodeKey);
        TopNode = res.Get<Node>(res.Nodes, TopNodeKey);
        Section = res.GetAndCast<Section::Section_CSTriangle>(res.Sections, SectionKey);
    }

    ValidationInfo CSTriangle::Validate() const
    {
        if (LeftNode == nullptr) return {"LeftNode is null"};
        if (RightNode == nullptr) return {"RightNode is null"};
        if (TopNode == nullptr) return {"TopNode is null"};
        if (Section == nullptr) return {"Section is null"};
        return {};
    }

    Numeric CSTriangle::GetTriangleArea() const
    {
        auto [xi, xj, xk] = GetTransformedX();
        auto [yi, yj, yk] = GetTransformedY();
        Numeric area = abs(static_cast<Numeric>(0.5) *
                           (xi * (yj - yk) +
                            xj * (yk - yi) +
                            xk * (yi - yj)));
        return area;
    }

    std::vector<ID> CSTriangle::GetNodeIds() const
    {
        return {LeftNode->Id, RightNode->Id, TopNode->Id};
    }

    [[nodiscard]] MatrixX CSTriangle::GetStiffnessLocal() const
    {
        auto DMatrix = GetDMatrix();
        auto BMatrix = GetBMatrix();

        Numeric A = GetTriangleArea();
        Numeric t = Section->Thickness;

        Numeric k = t / (4 * A);
        return k * BMatrix.transpose() * DMatrix * BMatrix;
    }

    MatrixX CSTriangle::GetStiffnessGlobal() const
    {
        auto& T = GetTransformMatrix();
        auto TExt = BlockDiagonal(T, 2 * GetNodeCount());
        return TExt.transpose() * GetStiffnessLocal() * TExt;
    }

    Matrix3x3 CSTriangle::GetDMatrix() const
    {
        Numeric v = Section->Mat->PoissonRation;
        Numeric E = Section->Mat->YoungsModules;
        if (!Section->IsConstantStrainTriangle) [[likely]]
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

    Eigen::Matrix<Numeric, 3, 18> CSTriangle::GetBMatrix() const
    {
        auto [xi, xj, xk] = GetTransformedX();
        auto [yi, yj, yk] = GetTransformedY();
        Numeric bi = yj - yk, bj = yk - yi, bk = yi - yj;
        Numeric gi = xk - xj, gj = xi - xk, gk = xj - xi;
        Eigen::Matrix<Numeric, 3, 18> BMatrix{{bi, 0, 0, 0, 0, 0, bj, 0, 0, 0, 0, 0, bk, 0, 0, 0, 0, 0},
                                              {0, gi, 0, 0, 0, 0, 0, gj, 0, 0, 0, 0, 0, gk, 0, 0, 0, 0},
                                              {gi, bi, 0, 0, 0, 0, gj, bj, 0, 0, 0, 0, gk, bk, 0, 0, 0, 0}};
        return BMatrix;
    }

    StressVector CSTriangle::CalculateStress(const VectorX& displacement) const
    {
        assert(displacement.size() == 18);
        StressVector result = StressVector::Zero();
        result({0, 1, 3}) = GetDMatrix() * GetBMatrix() * displacement;
        return result;
    }


    const Matrix3x3& CSTriangle::GetTransformMatrix() const
    {
        std::call_once(m_TransformMatrixFlag, [this]() {
            m_TransformMatrix = GetTransformationMatrixNature(*LeftNode, *RightNode, *TopNode);
        });
        return m_TransformMatrix;
    }

    const CSTriangle::Tuple3& CSTriangle::GetTransformedX() const
    {
        CalcTransformedXY();
        return m_TransformedX;
    }

    const CSTriangle::Tuple3& CSTriangle::GetTransformedY() const
    {
        CalcTransformedXY();
        return m_TransformedY;
    }

    void CSTriangle::CalcTransformedXY() const
    {
        std::call_once(m_TransformedXYFlag, [this]() {
            Matrix3x3 result;
            auto& T = GetTransformMatrix();
            result << ToVector(*LeftNode), ToVector(*RightNode), ToVector(*TopNode);
            result = T * result;
            m_TransformedX = ToTuple((Vector3)result.row(0));
            m_TransformedY = ToTuple((Vector3)result.row(1));
        });
    }

}// namespace Truss::Element
