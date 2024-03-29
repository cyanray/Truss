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
        ID YDirectionNodeKey{INVALID_ID};
        Node* LeftNode{};
        Node* RightNode{};
        /**
         * @brief YDirectionNode is the node that is used to indicate the direction of the Y axis.
         */
        Node* YDirectionNode{};
        Section::Section_Beam* Section{};

        void Build(Resources& resources) override;

        [[nodiscard]] ValidationInfo Validate() const override;

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

        [[nodiscard]] Numeric GetBeamLength() const;

        [[nodiscard]] std::vector<ID> GetNodeIds() const override;

        [[nodiscard]] MatrixX GetStiffnessLocal() const;

        [[nodiscard]] MatrixX GetStiffnessGlobal() const override;

        [[nodiscard]] StressVector CalculateStress(const VectorX& displacement) const override
        {
            return {};
        }

        [[nodiscard]] constexpr std::string GetElementName() const noexcept override
        {
            return "Beam";
        }

        ~Beam() override = default;
    };

}// namespace Truss::Element
