#pragma once
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"
#include "Truss/Material/Material_Elastic.hpp"
#include <Eigen/Dense>
#include <cmath>

namespace Truss::Element
{
	template<typename T = NumericType>
	class Element_Bar
	{
	public:
		int Id = INVALID_ID;
		T Area = 0;
		Material::Material_Elastic<T>* Mat = nullptr;
		PlaneNode<T>* LeftNode = nullptr;
		PlaneNode<T>* RightNode = nullptr;

		T GetBarLength()
		{
			return sqrt(pow(RightNode->X - LeftNode->X, 2) + pow(RightNode->Y - LeftNode->Y, 2));
		}


        constexpr int GetStiffnessSize() const noexcept
        {
            return 4;
        }

		Matrix4x4<T> GetStiffnessLocal()
		{
			auto E = Mat->YoungsModules;
			T bar_length = GetBarLength();
			T A = Area;
			Matrix4x4<T> result 
			{ 
				{  1, 0, -1, 0 }, 
				{  0, 0,  0, 0 }, 
				{ -1, 0,  1, 0 }, 
				{  0, 0,  0, 0 } 
			};
			return (E * A / bar_length) * result;
		}

		Matrix4x4<T> GetStiffnessGlobal()
		{
			T angle_rad = atan((RightNode->Y - LeftNode->Y) / (RightNode->X - LeftNode->X));
			auto transformation_matrix = GetCoordinateTransformationMatrix<T>(angle_rad);
			return transformation_matrix.transpose() * GetStiffnessLocal() * transformation_matrix;
		}


	};
}