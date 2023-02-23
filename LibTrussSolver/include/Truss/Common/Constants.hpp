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

	constexpr double PI_d = 3.1415926535897932384626;

    constexpr float PI_f = static_cast<float>(PI_d);

    constexpr int MAX_DOF = 6;

	template<typename T>
	using MatrixX = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

	template<typename T>
	using VectorX = Eigen::Vector<T, Eigen::Dynamic>;

    template<typename T>
    using Vector3 = Eigen::Vector<T, 3>;

    template<typename T>
    using Matrix3x3 = Eigen::Matrix<T, 3, 3>;

	template<typename T>
	using Matrix4x4 = Eigen::Matrix<T, 4, 4>;

    template<typename T>
    using Matrix6x6 = Eigen::Matrix<T, 6, 6>;

    template<typename T>
    using Matrix12x12 = Eigen::Matrix<T, 12, 12>;

	template<typename T, int NodeNumber>
	Eigen::Matrix<T, NodeNumber * 2, NodeNumber * 2> GetCoordinateTransformationMatrix(T angle_rad)
	{
		Matrix6x6<T> result
		{
			{  cos(angle_rad), sin(angle_rad), 0, 0 },
			{ -sin(angle_rad), cos(angle_rad), 0, 0 },
			{ 0, 0,  cos(angle_rad), sin(angle_rad) },
			{ 0, 0, -sin(angle_rad), cos(angle_rad) }
		};
		return Eigen::Matrix<T, NodeNumber * 2, NodeNumber * 2>::Zero();
	}

    template<typename T>
    Matrix3x3<T> GetTransformationMatrix(const Vector3<T>& XAsix)
    {
        Matrix3x3<T> result = Matrix3x3<T>::Zero();
        result.row(0) = XAsix.normalized();
        return result;
    }

    template<typename T>
    Matrix3x3<T> GetTransformationMatrix(const Vector3<T>& XAsix, const Vector3<T>& YAsix)
    {
        Matrix3x3<T> result = Matrix3x3<T>::Zero();
        result.row(0) = XAsix.normalized();
        result.row(1) = YAsix.normalized();
        return result;
    }

    template<typename T>
    Matrix3x3<T> GetTransformationMatrix(const Vector3<T>& XAsix, const Vector3<T>& YAsix, const Vector3<T>& ZAsix)
    {
        Matrix3x3<T> result = Matrix3x3<T>::Zero();
        result.row(0) = XAsix.normalized();
        result.row(1) = YAsix.normalized();
        result.row(2) = ZAsix.normalized();
        return result;
    }

    template <typename T, typename Derived>
    MatrixX<T> BlockDiagonal(const Eigen::MatrixBase<Derived>& mat, int count)
    {
        MatrixX<T> result = MatrixX<T>::Zero(mat.rows() * count, mat.cols() * count);
        for (int i = 0; i < count; ++i)
        {
            result.block(i * mat.rows(), i * mat.cols(), mat.rows(), mat.cols()) = mat;
        }

        return result;
    }

    enum class DegreeOfFreedom : char
    {
        Fixed = 0,
        X = 1,
        Y = 2,
        Z = 4,
        RX = 8,
        RY = 16,
        RZ = 32
    };

    constexpr DegreeOfFreedom operator|(DegreeOfFreedom lhs, DegreeOfFreedom rhs)
    {
        return static_cast<DegreeOfFreedom>(static_cast<char>(lhs) | static_cast<char>(rhs));
    }

    constexpr DegreeOfFreedom operator|=(DegreeOfFreedom& lhs, DegreeOfFreedom rhs)
    {
        lhs = (lhs | rhs);
        return lhs;
    }

    constexpr DegreeOfFreedom operator&(DegreeOfFreedom lhs, DegreeOfFreedom rhs)
    {
        return static_cast<DegreeOfFreedom>(static_cast<char>(lhs) & static_cast<char>(rhs));
    }

    class Resources;

}