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

        [[nodiscard]] Numeric GetBarLength() const;

        [[nodiscard]] constexpr int GetStiffnessSize() const noexcept;

        [[nodiscard]] Matrix4x4<Numeric> GetStiffnessLocal() const;

        [[nodiscard]] Matrix4x4<Numeric> _GetStiffnessGlobal() const;

        void Build(Resources& resources) override;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessGlobal() const override;

        std::vector<ID> GetNodeIds() const override;

    };
}