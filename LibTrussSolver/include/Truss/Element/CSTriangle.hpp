#pragma once
#include "ElementBase.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Section/Section_Beam.hpp"
#include "Truss/Section/Section_CSTriangle.hpp"

#include <mutex>

namespace Truss::Element
{

    class CSTriangle : public ElementBase
    {
    public:
        ID LeftNodeKey{INVALID_ID};
        ID RightNodeKey{INVALID_ID};
        ID TopNodeKey{INVALID_ID};
        ID SectionKey{INVALID_ID};
        Node* LeftNode{};
        Node* RightNode{};
        Node* TopNode{};
        Section::Section_CSTriangle* Section{};

    private:
        using Tuple3 = std::tuple<Numeric, Numeric, Numeric>;
        mutable Matrix3x3 m_TransformMatrix;
        mutable std::once_flag m_TransformMatrixFlag;
        mutable Tuple3 m_TransformedX;
        mutable Tuple3 m_TransformedY;
        mutable std::once_flag m_TransformedXYFlag;

        void CalcTransformedXY() const;

    public:
        void Build(Resources& res) override;

        [[nodiscard]] ValidationInfo Validate() const override;

        [[nodiscard]] constexpr int GetNodeCount() const noexcept override
        {
            return 3;
        }

        [[nodiscard]] constexpr DegreeOfFreedom GetNodeDegreeOfFreedom() const noexcept override
        {
            return DegreeOfFreedom::XYZ;
        }

        [[nodiscard]] constexpr int GetStiffnessSize() const noexcept override
        {
            return 18;
        }

        [[nodiscard]] Numeric GetTriangleArea() const;

        [[nodiscard]] std::vector<ID> GetNodeIds() const override;

        [[nodiscard]] MatrixX GetStiffnessLocal() const;

        [[nodiscard]] MatrixX GetStiffnessGlobal() const override;

        [[nodiscard]] Matrix3x3 GetDMatrix() const;

        [[nodiscard]] Eigen::Matrix<Numeric, 3, 18> GetBMatrix() const;

        [[nodiscard]] StressVector CalculateStress(const VectorX& displacement) const override;

        [[nodiscard]] constexpr std::string GetElementName() const noexcept override
        {
            return "CSTriangle";
        }

        const Matrix3x3& GetTransformMatrix() const;

        const Tuple3& GetTransformedX() const;

        const Tuple3& GetTransformedY() const;
    };

}// namespace Truss::Element