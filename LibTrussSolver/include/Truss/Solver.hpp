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

        std::vector<int> GetFreedomIndex();

    private:
        Resources m_Resources;

        [[nodiscard]] int GetNumberOfNode() const noexcept;

        void LoadNodes(const TrussDocument& doc);

        void LoadMaterials(const TrussDocument& doc);

        void LoadElements(const TrussDocument& doc);

        void LoadConstrains(const TrussDocument& doc);

        void LoadLoads(const TrussDocument& doc);

        void LoadSections(const TrussDocument& doc);

        void BuildAllComponents();

    };

}