#include "Truss/Serializer/Serializers.hpp"
#include <memory>
using namespace Truss;

void Truss::from_truss(const TrussDocument& doc, Node& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.X = doc["x"].GetOrDefault<Numeric>();
    obj.Y = doc["y"].GetOrDefault<Numeric>();
    obj.Z = doc["z"].GetOrDefault<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Constraint::NodeConstraint& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.XDisplacement = doc["xdisplacement"].GetOrDefault<bool>();
    obj.YDisplacement = doc["ydisplacement"].GetOrDefault<bool>();
    obj.ZDisplacement = doc["zdisplacement"].GetOrDefault<bool>();
    obj.XRotation = doc["xrotation"].GetOrDefault<bool>();
    obj.YRotation = doc["yrotation"].GetOrDefault<bool>();
    obj.ZRotation = doc["zrotation"].GetOrDefault<bool>();
    obj.NodeKey = doc["node_key"].Get<ID>();
}

void Truss::from_truss(const TrussDocument& doc, Element::Bar& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.LeftNodeKey = doc["node1_key"].Get<ID>();
    obj.RightNodeKey = doc["node2_key"].Get<ID>();
    obj.SectionKey = doc["section_key"].Get<ID>();
}

void Truss::from_truss(const TrussDocument& doc, Load::NodeLoad& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.NodeKey = doc["node_key"].Get<ID>();
    obj.YForce = doc["xforce"].GetOrDefault<Numeric>();
    obj.YForce = doc["yforce"].GetOrDefault<Numeric>();
    obj.ZForce = doc["zforce"].GetOrDefault<Numeric>();
    obj.XMoment = doc["xmoment"].GetOrDefault<Numeric>();
    obj.YMoment = doc["ymoment"].GetOrDefault<Numeric>();
    obj.ZMoment = doc["zmoment"].GetOrDefault<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Material::Elastic& obj)
{
    obj.Key = doc["key"].Get<ID>();

    obj.Density = doc["rho"].Get<Numeric>();
    obj.YoungsModules = doc["E"].Get<Numeric>();
    obj.PoissonRation = doc["pr"].Get<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Section::Section_Bar& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.MaterialKey = doc["mat_key"].Get<ID>();
    obj.Area = doc["area"].Get<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Element::Beam& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.LeftNodeKey = doc["node1_key"].Get<ID>();
    obj.RightNodeKey = doc["node2_key"].Get<ID>();
    obj.SectionKey = doc["section_key"].Get<ID>();
    obj.YNodeKey = doc["y_node_key"].GetOrDefault<ID>(INVALID_ID);
}

void Truss::from_truss(const TrussDocument& doc, Section::Section_Beam& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.MaterialKey = doc["mat_key"].Get<ID>();
    obj.Area = doc["area"].Get<Numeric>();
    obj.Iy = doc["Iy"].Get<Numeric>();
    obj.Iz = doc["Iz"].Get<Numeric>();
    obj.J = doc["J"].Get<Numeric>();
}


namespace
{
    template<typename TBase, typename T>
        requires std::derived_from<T, TBase>
    std::shared_ptr<TBase> Creator(const TrussDocument& doc)
    {
        auto value = std::make_shared<T>();
        from_truss(doc, *value);
        return std::static_pointer_cast<TBase>(value);
    }
}// namespace

SimpleReflection& Truss::GetCompomentReflection()
{
    static SimpleReflection refl;
    if (refl.IsEmpty()) [[unlikely]]
    {
        refl.Register("Elastic", Creator<Material::MaterialBase, Material::Elastic>);
        refl.Register("NodeConstraint", Creator<Constraint::ConstraintBase, Constraint::NodeConstraint>);
        refl.Register("Bar", Creator<Element::ElementBase, Element::Bar>);
        refl.Register("Beam", Creator<Element::ElementBase, Element::Beam>);
        refl.Register("NodeLoad", Creator<Load::LoadBase, Load::NodeLoad>);
        refl.Register("Section_Bar", Creator<Section::SectionBase, Section::Section_Bar>);
        refl.Register("Section_Beam", Creator<Section::SectionBase, Section::Section_Beam>);
    }
    return refl;
}
