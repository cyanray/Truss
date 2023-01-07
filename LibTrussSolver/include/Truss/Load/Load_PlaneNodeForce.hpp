#pragma once
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"


namespace Truss::Load
{
	template<typename T = NumericType>
	class Load_PlaneNodeForce
	{
    public:
        int Id = INVALID_ID;
        T XForce = 0;
        T YForce = 0;
        PlaneNode<T>* PlaneNode = nullptr;
	};
}