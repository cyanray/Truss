#pragma once
#include "Truss/Common/Constants.hpp"

namespace Truss::Material
{
	template<typename T = NumericType>
	struct Material_Elastic
	{
		int Id = INVALID_ID;
		T Density = 0;
		T YoungsModules = 0;
		T PoissonRation = 0;
	};
}