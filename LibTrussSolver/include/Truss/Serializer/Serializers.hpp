#pragma once
#include "TrussDocument/TrussDocument.hpp"
#include "Truss/Common/PlaneNode.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Constraint/PlaneNodeDisplacement.hpp"
#include "Truss/Element/PlaneBar.hpp"
#include "Truss/Load/PlaneNodeForce.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Section/Section_PlaneBar.hpp"

namespace Truss
{
    void from_truss(const TrussDocument& doc, PlaneNode& obj);
    void from_truss(const TrussDocument& doc, Node& obj);
    void from_truss(const TrussDocument& doc, Constraint::PlaneNodeDisplacement& obj);
    void from_truss(const TrussDocument& doc, Element::PlaneBar& obj);
    void from_truss(const TrussDocument& doc, Load::PlaneNodeForce& obj);
    void from_truss(const TrussDocument& doc, Material::Elastic& obj);
    void from_truss(const TrussDocument& doc, Section::Section_PlaneBar& obj);
}

