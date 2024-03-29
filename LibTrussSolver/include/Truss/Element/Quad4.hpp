#pragma once
#include "ElementBase.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Section/Section_Quad4.hpp"
#include <mutex>

namespace Truss::Element
{
    class Quad4 : public ElementBase
    {
    public:
        bool IsConstantStrainTriangle{false};
        ID Node1Key{INVALID_ID};
        ID Node2Key{INVALID_ID};
        ID Node3Key{INVALID_ID};
        ID Node4Key{INVALID_ID};
        ID SectionKey{INVALID_ID};
        Node* Node1{};
        Node* Node2{};
        Node* Node3{};
        Node* Node4{};
        Section::Section_Quad4* Section{};

    private:
        mutable Matrix3x3 m_TransformMatrix;
        mutable std::once_flag m_TransformMatrixFlag;
        mutable Vector4 m_TransformedX;
        mutable Vector4 m_TransformedY;
        mutable std::once_flag m_TransformedXYFlag;

        void CalcTransformedXY() const;

    public:
        void Build(Resources& resources) override;

        [[nodiscard]] ValidationInfo Validate() const override;

        [[nodiscard]] constexpr std::string GetElementName() const noexcept override
        {
            return "Quad4";
        }

        [[nodiscard]] constexpr int GetNodeCount() const noexcept override
        {
            return 4;
        }

        [[nodiscard]] constexpr DegreeOfFreedom GetNodeDegreeOfFreedom() const noexcept override
        {
            return DegreeOfFreedom::XYZ;
        }

        [[nodiscard]] constexpr int GetStiffnessSize() const noexcept override
        {
            return 24;
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override;

        [[nodiscard]] Numeric CalcJacobian(Numeric s, Numeric t) const;

        [[nodiscard]] Eigen::Matrix<Numeric, 3, 24> GetBMatrix(Numeric s, Numeric t, Numeric J) const;

        [[nodiscard]] Matrix3x3 GetDMatrix() const;

        [[nodiscard]] MatrixX GetStiffnessLocal() const;

        [[nodiscard]] MatrixX GetStiffnessGlobal() const override;

        [[nodiscard]] StressVector CalculateStress(const VectorX& displacement) const override;

        const Matrix3x3& GetTransformMatrix() const;

        const Vector4 & GetTransformedX() const;

        const Vector4 & GetTransformedY() const;

    };
}// namespace Truss::Element