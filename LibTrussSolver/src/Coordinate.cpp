#include "Truss/Common/Coordinate.hpp"

namespace Truss
{
    Matrix3x3 GetTransformationMatrix(const Truss::Vector3& XAxis, const Truss::Vector3& YAxis)
    {
        Matrix3x3 result = Matrix3x3::Zero();
        result.row(0) = XAxis.normalized();
        result.row(1) = YAxis.normalized();
        result.row(2) = XAxis.cross(YAxis).normalized();
        return result;
    }

    Matrix3x3 GetTransformationMatrixNature(const Truss::Vector3& XAxis)
    {
        Matrix3x3 result = Matrix3x3::Zero();
        Vector3 ZOrigin = Vector3::UnitZ(),
                YAxis = ZOrigin.cross(XAxis).normalized(),
                ZAxis = XAxis.cross(YAxis).normalized();
        result.row(0) = XAxis.normalized();
        result.row(1) = YAxis;
        result.row(2) = ZAxis;
        return result;
    }

}