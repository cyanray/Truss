#pragma once
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"
#include "Truss/Material/Elastic.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Truss::Element
{
	class Bar
	{
	public:
        ID Id = INVALID_ID;
		Numeric Area = 0;
		Material::Elastic* Mat = nullptr;
		PlaneNode* LeftNode = nullptr;
		PlaneNode* RightNode = nullptr;

		[[nodiscard]] Numeric GetBarLength() const;

        [[nodiscard]] constexpr int GetStiffnessSize() const noexcept;

		[[nodiscard]] Matrix4x4<Numeric> GetStiffnessLocal() const;

		[[nodiscard]] Matrix4x4<Numeric> GetStiffnessGlobal() const;

	};
}