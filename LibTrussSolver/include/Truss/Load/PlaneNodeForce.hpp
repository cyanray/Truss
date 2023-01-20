#pragma once
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"


namespace Truss::Load
{
	class PlaneNodeForce
	{
    public:
        ID Id = INVALID_ID;
        Numeric XForce = 0;
        Numeric YForce = 0;
        PlaneNode* PlaneNode = nullptr;
	};
}