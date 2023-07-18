#pragma once

#include "Node.hpp"
#include "Truss/Constraint/ConstraintBase.hpp"
#include "Truss/Element/ElementBase.hpp"
#include "Truss/Load/LoadBase.hpp"
#include "Truss/Material/Elastic.hpp"
#include "Truss/Section/SectionBase.hpp"
#include "Truss/Set/NodeSet.hpp"
#include <map>
#include <memory>

namespace Truss
{
    class Resources
    {
    public:
        std::map<ID, Node> Nodes;
        std::map<ID, Material::Elastic> Materials;
        std::map<ID, std::shared_ptr<Element::ElementBase>> Elements;
        std::map<ID, std::shared_ptr<Load::LoadBase>> Loads;
        std::map<ID, std::shared_ptr<Constraint::ConstraintBase>> Constraints;
        std::map<ID, std::shared_ptr<Section::SectionBase>> Sections;
        std::map<ID, std::shared_ptr<Set::SetBase>> Sets;

        template<typename T>
        T* Get(const std::map<ID, T>& from, ID key)
        {
            auto it = from.find(key);
            if (it == from.end()) return nullptr;
            return const_cast<T*>(&it->second);
        }

        template<typename TCast, typename TPtr>
        TCast* GetAndCast(const std::map<ID, std::shared_ptr<TPtr>>& from, ID key)
        {
            auto it = from.find(key);
            if (it == from.end()) return nullptr;
            return const_cast<TCast*>(std::static_pointer_cast<TCast>(it->second).get());
        }
    };
}// namespace Truss
