#pragma once
namespace Truss
{
    enum class DegreeOfFreedom : char
    {
        Fixed = 0,
        X = 1,
        Y = 2,
        Z = 4,
        RX = 8,
        RY = 16,
        RZ = 32
    };

    constexpr DegreeOfFreedom operator|(DegreeOfFreedom lhs, DegreeOfFreedom rhs)
    {
        return static_cast<DegreeOfFreedom>(static_cast<char>(lhs) | static_cast<char>(rhs));
    }

    constexpr DegreeOfFreedom operator|=(DegreeOfFreedom& lhs, DegreeOfFreedom rhs)
    {
        lhs = (lhs | rhs);
        return lhs;
    }

    constexpr DegreeOfFreedom operator&(DegreeOfFreedom lhs, DegreeOfFreedom rhs)
    {
        return static_cast<DegreeOfFreedom>(static_cast<char>(lhs) & static_cast<char>(rhs));
    }
}