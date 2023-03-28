#pragma once
#include <Eigen/Dense>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <tuple>

namespace Truss
{
    enum class GaussianPoint2D : uint8_t
    {
        One = 1,
        Four = 2,
        Nine = 3
    };

    template<typename T>
    inline std::tuple<Eigen::VectorX<T>, Eigen::MatrixX<T>>
    GetGaussWeightsAndPoints2D(GaussianPoint2D gaussPoint2D)
    {
        using namespace Eigen;
        // one
        static Vector<T, 1> weights_one = (Vector<T, 1>() << 4.0).finished();
        static Matrix<T, 1, 2> points_one = (Matrix<T, 1, 2>() << 0.0, 0.0).finished();
        // four
        static T t_four = 1 / std::sqrt(3);
        static Vector<T, 4> weights_four = (Vector<T, 4>() << 1.0, 1.0, 1.0, 1.0).finished();
        static Matrix<T, 4, 2> points_four = (Matrix<T, 4, 2>() << -t_four, -t_four, -t_four,
                                              t_four, t_four, -t_four, t_four, t_four)
                                                     .finished();
        // nine
        static Matrix<T, 9, 2> points_nine = []() {
            T t_nine = std::sqrt(3.0 / 5.0);
            Vector<T, 3> tmp;
            tmp << -t_nine, 0, t_nine;
            auto v = tmp.template replicate<3, 1>();
            auto k = tmp.rowwise().template replicate<3>().transpose().reshaped(9, 1);
            return (Matrix<T, 9, 2>() << k, v).finished();
        }();
        static Vector<T, 9> weights_nine = []() {
            Vector<T, 3> w;
            w << 5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0;
            return (w * w.transpose()).reshaped(9, 1);
        }();
        switch (gaussPoint2D)
        {
            case GaussianPoint2D::One:
            {
                return {weights_one, points_one};
            }
            case GaussianPoint2D::Four:
            {
                return {weights_four, points_four};
            }
            case GaussianPoint2D::Nine:
            {
                return {weights_nine, points_nine};
            }
        }
        throw std::runtime_error("Invalid GaussianPoint2D");
    }

    template<typename T>
    using IntegrateFunc = std::function<Eigen::VectorX<T>(const Eigen::VectorX<T>&, const Eigen::VectorX<T>&)>;

    template<typename T>
    inline T GaussianQuadrature2D(const IntegrateFunc<T>& func,
                                  Eigen::Matrix<T, 4, 2> vertices,
                                  GaussianPoint2D p = GaussianPoint2D::Four)
    {
        using namespace Eigen;
        auto [weights, points] = GetGaussWeightsAndPoints2D<T>(p);
        /* Shape functions */
        auto Psi1 = []<typename U>(U x, U y) constexpr { return (1 - x.array()) * (1 - y.array()) / 4; };
        auto Psi2 = []<typename U>(U x, U y) constexpr { return (1 + x.array()) * (1 - y.array()) / 4; };
        auto Psi3 = []<typename U>(U x, U y) constexpr { return (1 + x.array()) * (1 + y.array()) / 4; };
        auto Psi4 = []<typename U>(U x, U y) constexpr { return (1 - x.array()) * (1 + y.array()) / 4; };
        /* Shape function derivatives */
        auto dPsi11 = []<typename U>(U x, U y) constexpr { return -(1 - y) / 4; };
        auto dPsi12 = []<typename U>(U x, U y) constexpr { return -(1 - x) / 4; };
        auto dPsi21 = []<typename U>(U x, U y) constexpr { return (1 - y) / 4; };
        auto dPsi22 = []<typename U>(U x, U y) constexpr { return -(1 + x) / 4; };
        auto dPsi31 = []<typename U>(U x, U y) constexpr { return (1 + y) / 4; };
        auto dPsi32 = []<typename U>(U x, U y) constexpr { return (1 + x) / 4; };
        auto dPsi41 = []<typename U>(U x, U y) constexpr { return -(1 + y) / 4; };
        auto dPsi42 = []<typename U>(U x, U y) constexpr { return (1 - x) / 4; };
        /* Gradient matrix */
        auto Jacobian = [&]<typename U>(U x, U y) constexpr {
            Eigen::Matrix<T, 2, 4> result;
            result << dPsi11(x, y), dPsi21(x, y), dPsi31(x, y), dPsi41(x, y),
                    dPsi12(x, y), dPsi22(x, y), dPsi32(x, y), dPsi42(x, y);
            return result;
        };
        /* evaluate Shape functions on Gaussian reference points */
        auto xi = points(all, 0);
        auto eta = points(all, 1);
        auto evalPsi1 = Psi1(xi, eta);
        auto evalPsi2 = Psi2(xi, eta);
        auto evalPsi3 = Psi3(xi, eta);
        auto evalPsi4 = Psi4(xi, eta);
        /* from the change of variables function */
        auto ptGaussDomain = [&]() {
            MatrixX<T> result;
            result.resize(xi.size(), 4);
            result << evalPsi1, evalPsi2, evalPsi3, evalPsi4;
            return MatrixX<T>(result * vertices);
        }();
        /* evaluate Jacobian contribution for each point */
        auto evalDetJacb = [&]() {
            Matrix<T, Dynamic, 1> result;
            result.resize(xi.size());
            for (int i = 0; i < xi.size(); ++i)
            {
                result(i) = std::abs((Jacobian(xi(i), eta(i)) * vertices).determinant());
            }
            return result;
        }();
        /* evaluate the function on the domain points */
        auto evalF = func(ptGaussDomain(all, 0), ptGaussDomain(all, 1));
        T result = 0;
        for (int i = 0; i < ptGaussDomain.rows(); ++i)
        {
            result += weights(i) * evalF(i) * evalDetJacb(i);
        }
        return result;
    }
}// namespace Truss