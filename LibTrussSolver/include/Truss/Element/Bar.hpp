#pragma once

#include "ElementBase.hpp"
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/DegreeOfFreedom.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Section/Section_Bar.hpp"

namespace Truss::Element
{
    class Bar : public ElementBase
    {
    public:
        ID LeftNodeKey{INVALID_ID};
        ID RightNodeKey{INVALID_ID};
        ID SectionKey{INVALID_ID};
        Node* LeftNode{};
        Node* RightNode{};
        Section::Section_Bar* Section{};

        [[nodiscard]] constexpr int GetNodeCount() const noexcept override
        {
            return 2;
        }

        [[nodiscard]] constexpr int GetElementDofCount() const noexcept override
        {
            return 6;
        }

        [[nodiscard]] constexpr int GetNodeDofCount() const noexcept override
        {
            return 3;
        }

        [[nodiscard]] constexpr DegreeOfFreedom GetNodeDegreeOfFreedom() const noexcept override
        {
            return DegreeOfFreedom::XYZ;
        }

        [[nodiscard]] constexpr int GetStiffnessSize() const noexcept override
        {
            return 12;
        }

        [[nodiscard]] Numeric GetBarLength() const;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessLocal() const;

        void Build(Resources& resources) override;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessGlobal() const override;

        [[nodiscard]] std::vector<ID> GetNodeIds() const override;
    };
}// namespace Truss::Element