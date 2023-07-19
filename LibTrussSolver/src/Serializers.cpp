#include "Truss/Serializer/Serializers.hpp"
#include "TrussDocument/TrussDocument.hpp"
#include <memory>
using namespace Truss;

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
        refl.Register("NodeSetConstraint", Creator<Constraint::ConstraintBase, Constraint::NodeSetConstraint>);
        refl.Register("Bar", Creator<Element::ElementBase, Element::Bar>);
        refl.Register("Beam", Creator<Element::ElementBase, Element::Beam>);
        refl.Register("CSTriangle", Creator<Element::ElementBase, Element::CSTriangle>);
        refl.Register("Quad4", Creator<Element::ElementBase, Element::Quad4>);
        refl.Register("NodeLoad", Creator<Load::LoadBase, Load::NodeLoad>);
        refl.Register("BeamUniformLoad", Creator<Load::LoadBase, Load::BeamUniformLoad>);
        refl.Register("NodeSetLoad", Creator<Load::LoadBase, Load::NodeSetLoad>);
        refl.Register("Section_Bar", Creator<Section::SectionBase, Section::Section_Bar>);
        refl.Register("Section_Beam", Creator<Section::SectionBase, Section::Section_Beam>);
        refl.Register("Section_CSTriangle", Creator<Section::SectionBase, Section::Section_CSTriangle>);
        refl.Register("Section_Quad4", Creator<Section::SectionBase, Section::Section_Quad4>);
        refl.Register("NodeSet", Creator<Set::SetBase, Set::NodeSet>);
    }
    return refl;
}

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
    obj.XForce = doc["xforce"].GetOrDefault<Numeric>();
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
    obj.YDirectionNodeKey = doc["y_node_key"].GetOrDefault<ID>(INVALID_ID);
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

void Truss::from_truss(const TrussDocument& doc, Load::BeamUniformLoad& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.BeamKey = doc["beam_key"].Get<ID>();
    obj.YForce = doc["yforce"].GetOrDefault<Numeric>();
    obj.ZForce = doc["zforce"].GetOrDefault<Numeric>();
}


void Truss::from_truss(const TrussDocument& doc, Element::CSTriangle& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.LeftNodeKey = doc["node1_key"].Get<ID>();
    obj.RightNodeKey = doc["node2_key"].Get<ID>();
    obj.TopNodeKey = doc["node3_key"].Get<ID>();
    obj.SectionKey = doc["section_key"].Get<ID>();
}

void Truss::from_truss(const TrussDocument& doc, Section::Section_CSTriangle& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.MaterialKey = doc["mat_key"].Get<ID>();
    obj.Thickness = doc["thickness"].Get<Numeric>();
    obj.IsConstantStrainTriangle = doc["ConstantStrain"].GetOrDefault<bool>(false);
}

void Truss::from_truss(const TrussDocument& doc, Set::NodeSet& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.NodeKeys = doc["nodes"].Get<std::vector<ID>>();
}


void Truss::to_truss(TrussDocument& doc, const Node& obj)
{
    doc["key"] = obj.Key;
    doc["x"] = obj.X;
    doc["y"] = obj.Y;
    doc["z"] = obj.Z;
}

void Truss::to_truss(TrussDocument& doc, const Element::Bar& obj)
{
    doc["key"] = obj.Key;
    doc["node1_key"] = obj.LeftNodeKey;
    doc["node2_key"] = obj.RightNodeKey;
    doc["section_key"] = obj.SectionKey;
}

void Truss::to_truss(TrussDocument& doc, const Element::Beam& obj)
{
    doc["key"] = obj.Key;
    doc["node1_key"] = obj.LeftNodeKey;
    doc["node2_key"] = obj.RightNodeKey;
    doc["section_key"] = obj.SectionKey;
}

void Truss::to_truss(TrussDocument& doc, const Element::CSTriangle& obj)
{
    doc["key"] = obj.Key;
    doc["node1_key"] = obj.LeftNodeKey;
    doc["node2_key"] = obj.RightNodeKey;
    doc["node3_key"] = obj.TopNodeKey;
    doc["section_key"] = obj.SectionKey;
}

void Truss::to_truss(TrussDocument& doc, const Load::NodeLoad& obj)
{
    doc["key"] = obj.Key;
    doc["node_key"] = obj.NodeKey;
    doc["xforce"] = obj.XForce;
    doc["yforce"] = obj.YForce;
    doc["zforce"] = obj.ZForce;
    doc["xmoment"] = obj.XMoment;
    doc["ymoment"] = obj.YMoment;
    doc["zmoment"] = obj.ZMoment;
}

void Truss::to_truss(TrussDocument& doc, const Load::BeamUniformLoad& obj)
{
    doc["key"] = obj.Key;
    doc["beam_key"] = obj.BeamKey;
    doc["yforce"] = obj.YForce;
    doc["zforce"] = obj.ZForce;
}

void Truss::to_truss(TrussDocument& doc, const Constraint::NodeConstraint& obj)
{
    doc["key"] = obj.Key;
    doc["xdisplacement"] = obj.XDisplacement;
    doc["ydisplacement"] = obj.YDisplacement;
    doc["zdisplacement"] = obj.ZDisplacement;
    doc["xrotation"] = obj.XRotation;
    doc["yrotation"] = obj.YRotation;
    doc["zrotation"] = obj.ZRotation;
    doc["node_key"] = obj.NodeKey;
}

void Truss::to_truss(TrussDocument& doc, const Material::Elastic& obj)
{
    doc["key"] = obj.Key;

    doc["rho"] = obj.Density;
    doc["E"] = obj.YoungsModules;
    doc["pr"] = obj.PoissonRation;
}

void Truss::to_truss(TrussDocument& doc, const Section::Section_Bar& obj)
{
    doc["key"] = obj.Key;
    doc["mat_key"] = obj.MaterialKey;
    doc["area"] = obj.Area;
}

void Truss::to_truss(TrussDocument& doc, const Section::Section_Beam& obj)
{
    doc["key"] = obj.Key;
    doc["mat_key"] = obj.MaterialKey;
    doc["area"] = obj.Area;
    doc["Iy"] = obj.Iy;
    doc["Iz"] = obj.Iz;
    doc["J"] = obj.J;
}

void Truss::to_truss(TrussDocument& doc, const Section::Section_CSTriangle& obj)
{
    doc["key"] = obj.Key;
    doc["mat_key"] = obj.MaterialKey;
    doc["thickness"] = obj.Thickness;
    doc["ConstantStrain"] = obj.IsConstantStrainTriangle;
}

void Truss::to_truss(TrussDocument& doc, const Set::NodeSet& obj)
{
    doc["key"] = obj.Key;
    doc["nodes"] = TrussDocument(obj.NodeKeys);
}

void Truss::from_truss(const TrussDocument& doc, Load::NodeSetLoad& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.NodeSetKey = doc["node_set_key"].Get<ID>();
    obj.XForce = doc["xforce"].GetOrDefault<Numeric>();
    obj.YForce = doc["yforce"].GetOrDefault<Numeric>();
    obj.ZForce = doc["zforce"].GetOrDefault<Numeric>();
    obj.XMoment = doc["xmoment"].GetOrDefault<Numeric>();
    obj.YMoment = doc["ymoment"].GetOrDefault<Numeric>();
    obj.ZMoment = doc["zmoment"].GetOrDefault<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Constraint::NodeSetConstraint& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.NodeSetKey = doc["node_set_key"].Get<ID>();
    obj.XDisplacement = doc["xdisplacement"].GetOrDefault<bool>();
    obj.YDisplacement = doc["ydisplacement"].GetOrDefault<bool>();
    obj.ZDisplacement = doc["zdisplacement"].GetOrDefault<bool>();
    obj.XRotation = doc["xrotation"].GetOrDefault<bool>();
    obj.YRotation = doc["yrotation"].GetOrDefault<bool>();
    obj.ZRotation = doc["zrotation"].GetOrDefault<bool>();
}

void Truss::to_truss(TrussDocument& doc, const Load::NodeSetLoad& obj)
{
    doc["key"] = obj.Key;
    doc["node_set_key"] = obj.NodeSetKey;
    doc["xforce"] = obj.XForce;
    doc["yforce"] = obj.YForce;
    doc["zforce"] = obj.ZForce;
    doc["xmoment"] = obj.XMoment;
    doc["ymoment"] = obj.YMoment;
    doc["zmoment"] = obj.ZMoment;
}

void Truss::to_truss(TrussDocument& doc, const Constraint::NodeSetConstraint& obj)
{
    doc["key"] = obj.Key;
    doc["node_set_key"] = obj.NodeSetKey;
    doc["xdisplacement"] = obj.XDisplacement;
    doc["ydisplacement"] = obj.YDisplacement;
    doc["zdisplacement"] = obj.ZDisplacement;
    doc["xrotation"] = obj.XRotation;
    doc["yrotation"] = obj.YRotation;
    doc["zrotation"] = obj.ZRotation;
}

void Truss::from_truss(const TrussDocument& doc, Element::Quad4& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.SectionKey = doc["section_key"].Get<ID>();
    obj.Node1Key = doc["node1_key"].Get<ID>();
    obj.Node2Key = doc["node2_key"].Get<ID>();
    obj.Node3Key = doc["node3_key"].Get<ID>();
    obj.Node4Key = doc["node4_key"].Get<ID>();
}

void Truss::from_truss(const TrussDocument& doc, Section::Section_Quad4& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.MaterialKey = doc["mat_key"].Get<ID>();
    obj.Thickness = doc["thickness"].Get<Numeric>();
}

void Truss::to_truss(TrussDocument& doc, const Element::Quad4& obj)
{
    doc["key"] = obj.Key;
    doc["section_key"] = obj.SectionKey;
    doc["node1_key"] = obj.Node1Key;
    doc["node2_key"] = obj.Node2Key;
    doc["node3_key"] = obj.Node3Key;
    doc["node4_key"] = obj.Node4Key;
}

void Truss::to_truss(TrussDocument& doc, const Section::Section_Quad4& obj)
{
    doc["key"] = obj.Key;
    doc["mat_key"] = obj.MaterialKey;
    doc["thickness"] = obj.Thickness;
}
