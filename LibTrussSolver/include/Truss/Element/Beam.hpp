#pragma once
#include "ElementBase.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Section/Section_Beam.hpp"

namespace Truss::Element
{

    class Beam : public ElementBase
    {
    public:
        ID LeftNodeKey{INVALID_ID};
        ID RightNodeKey{INVALID_ID};
        ID SectionKey{INVALID_ID};
        Node* LeftNode{};
        Node* RightNode{};
        Section::Section_Beam* Section{};

        void Build(Resources& resources) override
        {
        }

        [[nodiscard]] constexpr int GetNodeCount() const noexcept override
        {
            return 2;
        }

        [[nodiscard]] constexpr DegreeOfFreedom GetNodeDegreeOfFreedom() const noexcept override
        {
            return DegreeOfFreedom::ALL;
        }

        [[nodiscard]] constexpr int GetStiffnessSize() const noexcept override
        {
            return 12;
        }

        [[nodiscard]] std::vector<ID> GetNodeIds() const override;

        [[nodiscard]] MatrixX<Numeric> GetStiffnessGlobal() const override
        {
            return Truss::MatrixX<Numeric>();
        }
    };

}

