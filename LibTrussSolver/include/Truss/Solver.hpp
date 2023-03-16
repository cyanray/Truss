#pragma once

#include "Truss/Common/Constants.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Common/Resources.hpp"
#include "Truss/Constraint/NodeConstraint.hpp"
#include "Truss/Element/Bar.hpp"
#include "Truss/Load/NodeLoad.hpp"
#include "Truss/Material/Elastic.hpp"
#include "TrussDocument/TrussDocument.hpp"
#include <Eigen/Dense>
#include <map>
#include <string>
#include <vector>


namespace Truss
{
    class TrussSolver
    {
    public:
        void LoadTrussDocument(const TrussDocument& doc);

        VectorX<Numeric> GetF();

        VectorX<Numeric> GetF(const std::vector<int>& index);

        MatrixX<Numeric> GetK();

        MatrixX<Numeric> GetK(const std::vector<int>& index);

        [[nodiscard]] int GetKSize() const noexcept;

        std::vector<int> GetFreedomIndex();

    private:
        Resources m_Resources;

        [[nodiscard]] int GetNumberOfNode() const noexcept;

        void BuildAllComponents();
    };

}// namespace Truss