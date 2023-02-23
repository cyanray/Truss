#include "Truss/Serializer/Serializers.hpp"
using namespace Truss;

void Truss::from_truss(const TrussDocument& doc, Node& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.X = doc["x"].GetOrDefault<Numeric>();
    obj.Y = doc["y"].GetOrDefault<Numeric>();
    obj.Z = doc["z"].GetOrDefault<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Constraint::NodeDisplacement& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.XConstraint = doc["x"].GetOrDefault<bool>();
    obj.YConstraint = doc["y"].GetOrDefault<bool>();
    obj.ZConstraint = doc["z"].GetOrDefault<bool>();
    obj.NodeKey = doc["node_key"].Get<ID>();
}

void Truss::from_truss(const TrussDocument& doc, Element::Bar& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.LeftNodeKey = doc["node1_key"].Get<ID>();
    obj.RightNodeKey = doc["node2_key"].Get<ID>();
    obj.SectionKey = doc["section_key"].Get<ID>();
}

void Truss::from_truss(const TrussDocument& doc, Load::NodeForce& obj)
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
