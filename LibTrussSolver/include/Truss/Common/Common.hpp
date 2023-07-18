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

    const static Eigen::Array<int, 6, 1> DOF_INDEX{0, 1, 2, 3, 4, 5};

    using MatrixX = Eigen::Matrix<Numeric, Eigen::Dynamic, Eigen::Dynamic>;

    using VectorX = Eigen::Vector<Numeric, Eigen::Dynamic>;

    using Vector3 = Eigen::Vector<Numeric, 3>;

    using Matrix3x3 = Eigen::Matrix<Numeric, 3, 3>;

    using Matrix6x6 = Eigen::Matrix<Numeric, 6, 6>;

    using Matrix12x12 = Eigen::Matrix<Numeric, 12, 12>;

    using LoadVector = Eigen::Vector<Numeric, ALL_DOF>;

    using DisplacementVector = Eigen::Vector<Numeric, ALL_DOF>;

    using StressVector = Eigen::Vector<Numeric, ALL_DOF>;

    template<typename Derived>
    MatrixX BlockDiagonal(const Eigen::MatrixBase<Derived>& mat, int count)
    {
        MatrixX result = MatrixX::Zero(mat.rows() * count, mat.cols() * count);
        for (int i = 0; i < count; ++i)
        {
            result.block(i * mat.rows(), i * mat.cols(), mat.rows(), mat.cols()) = mat;
        }
        return result;
    }

    template<typename TContainer>
    std::vector<int> GenerateAllDOFIndex(const TContainer& id_list)
    {
        std::vector<int> index;
        index.reserve(id_list.size() * ALL_DOF);
        for (int id: id_list)
        {
            auto tmp = DOF_INDEX + id * ALL_DOF;
            index.insert(index.end(), tmp.begin(), tmp.end());
        }
        return index;
    }

    /**
     * Forward declaration.
     */
    class Resources;

}// namespace Truss