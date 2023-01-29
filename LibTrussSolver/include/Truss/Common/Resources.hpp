#pragma once

#include <map>
#include <memory>
#include "PlaneNode.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Section/Section_PlaneBar.hpp"
#include "Truss/Element/ElementBase.hpp"
#include "Truss/Load/LoadBase.hpp"
#include "Truss/Constraint/ConstraintBase.hpp"

namespace Truss
{
    class Resources
    {
    public:
        std::map<ID, PlaneNode> PlaneNodes;
        std::map<ID, Material::Elastic> Materials;
        std::map<ID, std::shared_ptr<Element::ElementBase>> Elements;
        std::map<ID, std::shared_ptr<Load::LoadBase>> Loads;
        std::map<ID, std::shared_ptr<Constraint::ConstraintBase>> Constraints;
        std::map<ID, Section::Section_PlaneBar> Sections;
    };
}
