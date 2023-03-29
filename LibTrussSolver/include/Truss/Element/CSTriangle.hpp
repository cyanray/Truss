#pragma once
#include "ElementBase.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Section/Section_Beam.hpp"
#include "Truss/Section/Section_CSTriangle.hpp"

namespace Truss::Element
{

    class CSTriangle : public ElementBase
    {
    public:
        bool IsConstantStrainTriangle{false};
        ID LeftNodeKey{INVALID_ID};
        ID RightNodeKey{INVALID_ID};
        ID TopNodeKey{INVALID_ID};
        ID SectionKey{INVALID_ID};
        Node* LeftNode{};
        Node* RightNode{};
        Node* TopNode{};
        Section::Section_CSTriangle* Section{};

        void Build(Resources& resources) override;

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

        [[nodiscard]] MatrixX<Numeric> GetStiffnessLocal() const;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessGlobal() const override;

        [[nodiscard]] Matrix3x3<Numeric> GetDMatrix() const;

        [[nodiscard]] Eigen::Matrix<Numeric, 3, 18> GetBMatrix() const;

        [[nodiscard]] StressVector CalculateStress(const VectorX<Numeric>& displacement) const override;

        [[nodiscard]] constexpr std::string GetElementName() const noexcept override
        {
            return "CSTriangle";
        }
    };

}// namespace Truss::Element