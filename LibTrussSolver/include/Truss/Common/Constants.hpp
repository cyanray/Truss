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

    constexpr int ALL_DOF = 6;

	template<typename T>
	using MatrixX = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

	template<typename T>
	using VectorX = Eigen::Vector<T, Eigen::Dynamic>;

    template<typename T>
    using Vector3 = Eigen::Vector<T, 3>;

    template<typename T>
    using Matrix3x3 = Eigen::Matrix<T, 3, 3>;

    template<typename T>
    using Matrix6x6 = Eigen::Matrix<T, 6, 6>;

    template<typename T>
    using Matrix12x12 = Eigen::Matrix<T, 12, 12>;


    template<typename T>
    Matrix3x3<T> GetTransformationMatrix(
        const Vector3<T>& XAsix, 
        const Vector3<T>& YAsix = Vector3<T>::Zero(),
        const Vector3<T>& ZAsix = Vector3<T>::Zero())
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

    class Resources;

}