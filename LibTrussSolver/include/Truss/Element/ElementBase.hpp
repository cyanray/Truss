#pragma once

#include "Truss/Common/IComponent.hpp"
#include <bit>

namespace Truss::Element
{

    class ElementBase : public IComponent
    {
    public:
        virtual void Build(Resources&) = 0;

        [[nodiscard]] constexpr virtual int GetNodeCount() const noexcept = 0;

        [[nodiscard]] constexpr virtual int GetElementDofCount() const noexcept
        {
            return GetNodeCount() * GetNodeDofCount();
        }

        [[nodiscard]] constexpr virtual int GetNodeDofCount() const noexcept
        {
            return std::popcount(static_cast<uint8_t>(GetNodeDegreeOfFreedom()));
        }

        [[nodiscard]] constexpr virtual DegreeOfFreedom GetNodeDegreeOfFreedom() const noexcept = 0;

        [[nodiscard]] constexpr virtual int GetStiffnessSize() const noexcept = 0;

        /**
         *  must be sorted
         */
        [[nodiscard]] virtual std::vector<ID> GetNodeIds() const = 0;

        [[nodiscard]] virtual MatrixX<Numeric> GetStiffnessGlobal() const = 0;

    };

}