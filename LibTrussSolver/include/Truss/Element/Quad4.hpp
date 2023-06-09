#pragma once
#include "ElementBase.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Section/Section_CSQuad.hpp"

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
        Section::Section_CSQuad* Section{};

        ~Quad4() override = default;

        [[nodiscard]] constexpr std::string GetElementName() const noexcept override
        {
            return "Quad4";
        }

        void Build(Resources& resources) override;

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

        [[nodiscard]] Eigen::Matrix<Numeric, 3, 24> GetBMatrix(const VectorX<Numeric>& x, const VectorX<Numeric>& y) const;

        [[nodiscard]] Matrix3x3<Numeric> GetDMatrix() const;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessLocal() const;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessGlobal() const override;

        [[nodiscard]] StressVector CalculateStress(const VectorX<Numeric>& displacement) const override;
    };
}// namespace Truss::Element