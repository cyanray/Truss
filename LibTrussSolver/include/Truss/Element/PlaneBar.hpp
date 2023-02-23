#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Section/Section_PlaneBar.hpp"
#include "ElementBase.hpp"

namespace Truss::Element
{
    class PlaneBar : public ElementBase
    {
    public:
        ID LeftNodeKey{INVALID_ID};
        ID RightNodeKey{INVALID_ID};
        ID SectionKey{INVALID_ID};
        PlaneNode* LeftNode{};
        PlaneNode* RightNode{};
        Section::Section_PlaneBar* Section{};

        [[nodiscard]] constexpr virtual int GetNodeCount() const noexcept override
        {
            return 2;
        }

        [[nodiscard]] constexpr virtual int GetElementDofCount() const noexcept override
        {
            return 4;
        }

        [[nodiscard]] constexpr virtual int GetNodeDofCount() const noexcept override
        {
            return 2;
        }

        [[nodiscard]] constexpr virtual DegreeOfFreedom GetNodeDegreeOfFreedom() const noexcept override
        {
            return DegreeOfFreedom::X | DegreeOfFreedom::Y;
        }

        [[nodiscard]] constexpr virtual int GetStiffnessSize() const noexcept override
        {
            return 6;
        }

        [[nodiscard]] Numeric GetBarLength() const;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessLocal() const;

        void Build(Resources& resources) override;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessGlobal() const override;

        [[nodiscard]] std::vector<ID> GetNodeIds() const override;

    };
}