#pragma once
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"


namespace Truss::Constraint
{
    template<typename T = NumericType>
    class Constraint_PlaneNodeDisplacement
    {
    public:
        int Id = INVALID_ID;
        bool XConstraint = false;
        bool YConstraint = false;
        PlaneNode<T>* PlaneNode = nullptr;
    };
}