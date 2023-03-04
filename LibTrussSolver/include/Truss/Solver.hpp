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

        VectorX<Numeric> GetSimplifiedF();

        MatrixX<Numeric> GetK();

        MatrixX<Numeric> GetSimplifiedK();

        [[nodiscard]] int GetKSize() const noexcept;

        std::vector<int> GetSimplifiedIndex();

    private:
        Resources m_Resources;

        [[nodiscard]] int GetNumberOfNode() const noexcept;

        void GetNodes(const TrussDocument& doc);

        void GetMaterials(const TrussDocument& doc);

        void GetElements(const TrussDocument& doc);

        void GetConstrains(const TrussDocument& doc);

        void GetLoads(const TrussDocument& doc);

        void GetSections(const TrussDocument& doc);

        void BuildAllComponents();

    };

}