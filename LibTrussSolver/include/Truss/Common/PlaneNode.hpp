#pragma once
#include "Constants.hpp"

namespace Truss
{
	template<typename T = NumericType>
	struct PlaneNode
	{
		int Id = INVALID_ID;
		T X = 0;
		T Y = 0;
	};
}