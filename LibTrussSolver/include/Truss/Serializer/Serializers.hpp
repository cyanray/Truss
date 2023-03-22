#pragma once
#include "Truss/Common/Node.hpp"
#include "Truss/Constraint/NodeConstraint.hpp"
#include "Truss/Element/Bar.hpp"
#include "Truss/Element/Beam.hpp"
#include "Truss/Element/CSTriangle.hpp"
#include "Truss/Load/BeamUniformLoad.hpp"
#include "Truss/Load/NodeLoad.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Section/Section_Bar.hpp"
#include "Truss/Section/Section_Beam.hpp"
#include "Truss/Section/Section_CSTriangle.hpp"
#include "Truss/Set/NodeSet.hpp"
#include "Truss/Utils/SimpleReflection.hpp"

namespace Truss
{
    class TrussDocument;

    // from_truss
    void from_truss(const TrussDocument& doc, Node& obj);
    void from_truss(const TrussDocument& doc, Element::Bar& obj);
    void from_truss(const TrussDocument& doc, Element::Beam& obj);
    void from_truss(const TrussDocument& doc, Element::CSTriangle& obj);
    void from_truss(const TrussDocument& doc, Load::NodeLoad& obj);
    void from_truss(const TrussDocument& doc, Load::BeamUniformLoad& obj);
    void from_truss(const TrussDocument& doc, Constraint::NodeConstraint& obj);
    void from_truss(const TrussDocument& doc, Material::Elastic& obj);
    void from_truss(const TrussDocument& doc, Section::Section_Bar& obj);
    void from_truss(const TrussDocument& doc, Section::Section_Beam& obj);
    void from_truss(const TrussDocument& doc, Section::Section_CSTriangle& obj);
    void from_truss(const TrussDocument& doc, Set::NodeSet& obj);

    // to_truss
    void to_truss(TrussDocument& doc, const Node& obj);
    void to_truss(TrussDocument& doc, const Element::Bar& obj);
    void to_truss(TrussDocument& doc, const Element::Beam& obj);
    void to_truss(TrussDocument& doc, const Element::CSTriangle& obj);
    void to_truss(TrussDocument& doc, const Load::NodeLoad& obj);
    void to_truss(TrussDocument& doc, const Load::BeamUniformLoad& obj);
    void to_truss(TrussDocument& doc, const Constraint::NodeConstraint& obj);
    void to_truss(TrussDocument& doc, const Material::Elastic& obj);
    void to_truss(TrussDocument& doc, const Section::Section_Bar& obj);
    void to_truss(TrussDocument& doc, const Section::Section_Beam& obj);
    void to_truss(TrussDocument& doc, const Section::Section_CSTriangle& obj);

    SimpleReflection& GetCompomentReflection();
}// namespace Truss
