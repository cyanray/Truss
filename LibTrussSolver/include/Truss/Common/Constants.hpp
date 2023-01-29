#pragma once
#include <Eigen/Dense>
#include <cmath>

namespace Truss
{
#ifdef TRUSS_DOUBLE_PRECISION
	using Numeric = double;
#else
	using Numeric = float;
#endif

    using ID = int;

	constexpr ID INVALID_ID = -1;

	constexpr float PI_f = 3.14159265358979323846f;

	constexpr double PI_d = 3.14159265358979323846;

	template<typename T>
	using MatrixX = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

	template<typename T>
	using VectorX = Eigen::Vector<T, Eigen::Dynamic>;

	template<typename T>
	using Matrix4x4 = Eigen::Matrix<T, 4, 4>;

	template<typename T>
	Matrix4x4<T> GetCoordinateTransformationMatrix(T angle_rad)
	{
		Matrix4x4<T> result 
		{
			{  cos(angle_rad), sin(angle_rad), 0, 0 },
			{ -sin(angle_rad), cos(angle_rad), 0, 0 },
			{ 0, 0,  cos(angle_rad), sin(angle_rad) },
			{ 0, 0, -sin(angle_rad), cos(angle_rad) }
		};
		return result;
	}

    class Resources;

}