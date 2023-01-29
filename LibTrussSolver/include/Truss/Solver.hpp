#pragma once

#include <map>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "Truss/Common/Constants.hpp"
#include "Truss/Common/PlaneNode.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Element/PlaneBar.hpp"
#include "Truss/Constraint/PlaneNodeDisplacement.hpp"
#include "Truss/Load/PlaneNodeForce.hpp"
#include "TrussDocument/TrussDocument.hpp"
#include "Truss/Common/Resources.hpp"


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

        void GetPlaneNodes(const TrussDocument& doc);

        void GetMaterials(const TrussDocument& doc);

        void GetElements(const TrussDocument& doc);

        void GetConstrains(const TrussDocument& doc);

        void GetLoads(const TrussDocument& doc);

        void GetSections(const TrussDocument& doc);

        void BuildAllComponents();

    };

}