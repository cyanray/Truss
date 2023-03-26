#pragma once
#include <Eigen/Dense>
#include <format>
#include <string>
#include "CsvHeaders.hpp"
#include "Csv/CsvToString.hpp"
using std::string;

namespace Csv
{
    template<typename T, int Row, int Col>
    inline std::string csv_to_string(const Eigen::Matrix<T, Row, Col>& matrix)
    {
        string result;
        auto row = matrix.rows();
        auto col = matrix.cols();
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < col; ++j)
            {
                result += csv_to_string(T(matrix(i, j)));
                result += ",";
            }
        }
        result.pop_back();
        return result;
    }

}