#pragma once
#include "Truss/Common/Node.hpp"
#include "Truss/Constraint/NodeConstraint.hpp"
#include "Truss/Element/Bar.hpp"
#include "Truss/Load/NodeLoad.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Section/Section_Bar.hpp"
#include "TrussDocument/TrussDocument.hpp"

namespace Truss
{
    void from_truss(const TrussDocument& doc, Node& obj);
    void from_truss(const TrussDocument& doc, Constraint::NodeConstraint& obj);
    void from_truss(const TrussDocument& doc, Element::Bar& obj);
    void from_truss(const TrussDocument& doc, Load::NodeLoad& obj);
    void from_truss(const TrussDocument& doc, Material::Elastic& obj);
    void from_truss(const TrussDocument& doc, Section::Section_Bar& obj);
}

