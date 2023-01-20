#pragma once
#include "Truss/Common/Constants.hpp"

namespace Truss::Material
{
	struct Elastic
	{
        ID Id = INVALID_ID;
		Numeric Density = 0;
		Numeric YoungsModules = 0;
		Numeric PoissonRation = 0;
	};
}