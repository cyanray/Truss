#include "Truss/Element/Quad4.hpp"
#include "Truss/Common/Coordinate.hpp"
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
        auto& T = GetTransformMatrix();
        auto TExt = BlockDiagonal(T, 2 * GetNodeCount());
        return TExt.transpose() * GetStiffnessLocal() * TExt;
    }

    StressVector Quad4::CalculateStress(const VectorX& displacement) const
    {
        return {};
    }

    MatrixX Quad4::GetStiffnessLocal() const
    {
        Eigen::Matrix<Numeric, 24, 24> ke = Eigen::Matrix<Numeric, 24, 24>::Zero();
        auto [weights, points] = GetGaussWeightsAndPoints2D(GaussianPoint2D::Four);
        auto D = GetDMatrix();
        Numeric h = Section->Thickness;
        for (int i = 0; i < weights.size(); ++i)
        {
            Numeric w = weights(i);
            Numeric s = points(i, 0), t = points(i, 1);
            auto J = CalcJacobian(s, t);
            auto B = GetBMatrix(s, t, J);
            ke = ke + (B.transpose() * D * B) * w * J * h;
        }
        return ke;
    }

    Numeric Quad4::CalcJacobian(Numeric s, Numeric t) const
    {
        Numeric result;
        auto& x = GetTransformedX();
        auto& y = GetTransformedY();
        Eigen::Matrix<Numeric, 4, 4> T;
        T << 0, 1 - t, t - s, s - 1,
            t - 1, 0, s + 1, -s - t,
            s - t, -s - 1, 0, t + 1,
            1 - s, s + t, -t - 1, 0;
        result = 1.0 / 8.0 * (x.transpose() * T * y).value();
        return result;
    }

    Eigen::Matrix<Numeric, 3, 24> Quad4::GetBMatrix(Numeric s, Numeric t, Numeric J) const
    {
        Eigen::Matrix<Numeric, 3, 24> result = Eigen::Matrix<Numeric, 3, 24>::Zero();
        auto& x = GetTransformedX();
        auto& y = GetTransformedY();
        Eigen::Matrix<Numeric, 4, 2> dN;
        Eigen::Matrix<Numeric, 4, 2> xy;
        xy << x, y;
        dN << t - 1, s - 1,
              1 - t, -(1 + s),
              1 + t, 1 + s,
              -(1 + t), 1 - s;
        dN = dN / 4.0;
        auto [c,d,b,a] = ToTuple((Vector4)((xy.transpose() * dN).reshaped(4, 1)));
        auto Bi = [&](int t){
            Eigen::Matrix<Numeric, 3, 2> result;
            result << (a * dN(t, 0) - b * dN(t, 1)), 0,
                      0, (c * dN(t, 1) - d * dN(t, 0)),
                      (c * dN(t, 1) - d * dN(t, 0)), (a * dN(t, 0) - b * dN(t, 1));
            return result;
        };
        result(Eigen::all, {0, 1}) = Bi(0);
        result(Eigen::all, {6, 7}) = Bi(1);
        result(Eigen::all, {12, 13}) = Bi(2);
        result(Eigen::all, {18, 19}) = Bi(3);
        result = result / J;
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
    const Matrix3x3& Quad4::GetTransformMatrix() const
    {
        std::call_once(m_TransformMatrixFlag, [this]() {
            m_TransformMatrix = GetTransformationMatrixNature(*Node1, *Node2, *Node3);
        });
        return m_TransformMatrix;
    }

    const Vector4& Quad4::GetTransformedX() const
    {
        CalcTransformedXY();
        return m_TransformedX;
    }

    const Vector4& Quad4::GetTransformedY() const
    {
        CalcTransformedXY();
        return m_TransformedY;
    }

    void Quad4::CalcTransformedXY() const
    {
        std::call_once(m_TransformedXYFlag, [this]() {
            Eigen::Matrix<Numeric, 3, 4> result;
            auto& T = GetTransformMatrix();
            result << ToVector(*Node1), ToVector(*Node2), ToVector(*Node3), ToVector(*Node4);
            result = T * result;
            m_TransformedX = result.row(0);
            m_TransformedY = result.row(1);
        });
    }
}// namespace Truss::Element