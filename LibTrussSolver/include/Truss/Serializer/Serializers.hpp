#pragma once
#include "TrussDocument/TrussDocument.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Common/Node.hpp"
#include "Truss/Constraint/NodeDisplacement.hpp"
#include "Truss/Element/Bar.hpp"
#include "Truss/Load/NodeForce.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Section/Section_Bar.hpp"

namespace Truss
{
    void from_truss(const TrussDocument& doc, Node& obj);
    void from_truss(const TrussDocument& doc, Constraint::NodeDisplacement& obj);
    void from_truss(const TrussDocument& doc, Element::Bar& obj);
    void from_truss(const TrussDocument& doc, Load::NodeForce& obj);
    void from_truss(const TrussDocument& doc, Material::Elastic& obj);
    void from_truss(const TrussDocument& doc, Section::Section_Bar& obj);
}

