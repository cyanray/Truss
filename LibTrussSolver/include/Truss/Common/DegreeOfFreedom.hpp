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
        RZ = 32,
        XYZ = X | Y | Z,
        RXRYRZ = RX | RY | RZ,
        ALL = XYZ | RXRYRZ
    };

    constexpr DegreeOfFreedom operator|(DegreeOfFreedom lhs, DegreeOfFreedom rhs)
    {
        return static_cast<DegreeOfFreedom>(static_cast<char>(lhs) | static_cast<char>(rhs));
    }

    constexpr void operator|=(DegreeOfFreedom& lhs, DegreeOfFreedom rhs)
    {
        lhs = (lhs | rhs);
    }

    constexpr DegreeOfFreedom operator&(DegreeOfFreedom lhs, DegreeOfFreedom rhs)
    {
        return static_cast<DegreeOfFreedom>(static_cast<char>(lhs) & static_cast<char>(rhs));
    }
}