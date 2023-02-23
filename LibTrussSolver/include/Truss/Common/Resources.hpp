#pragma once

#include <map>
#include <memory>
#include "PlaneNode.hpp"
#include "Node.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Element/ElementBase.hpp"
#include "Truss/Load/LoadBase.hpp"
#include "Truss/Constraint/ConstraintBase.hpp"
#include "Truss/Section/SectionBase.hpp"

namespace Truss
{
    class Resources
    {
    public:
        std::map<ID, PlaneNode> PlaneNodes;
        std::map<ID, Node> Nodes;
        std::map<ID, Material::Elastic> Materials;
        std::map<ID, std::shared_ptr<Element::ElementBase>> Elements;
        std::map<ID, std::shared_ptr<Load::LoadBase>> Loads;
        std::map<ID, std::shared_ptr<Constraint::ConstraintBase>> Constraints;
        std::map<ID, std::shared_ptr<Section::SectionBase>> Sections;
    };
}
