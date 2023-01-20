#pragma once
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"


namespace Truss::Constraint
{
    class PlaneNodeDisplacement
    {
    public:
        ID Id = INVALID_ID;
        bool XConstraint = false;
        bool YConstraint = false;
        PlaneNode* PlaneNode = nullptr;
    };
}