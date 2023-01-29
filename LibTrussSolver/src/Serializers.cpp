#include "Truss/Serializer/Serializers.hpp"
using namespace Truss;

void Truss::from_truss(const TrussDocument &doc, PlaneNode &obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.X = doc["x"].Get<Numeric>();
    obj.Y = doc["y"].Get<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Node& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.X = doc["x"].Get<Numeric>();
    obj.Y = doc["y"].Get<Numeric>();
    obj.Z = doc["z"].Get<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Constraint::PlaneNodeDisplacement& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.XConstraint = doc["x"].Get<bool>();
    obj.YConstraint = doc["y"].Get<bool>();
    obj.PlaneNodeKey = doc["node_key"].Get<ID>();
}

void Truss::from_truss(const TrussDocument& doc, Element::PlaneBar& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.LeftNodeKey = doc["node1_key"].Get<ID>();
    obj.RightNodeKey = doc["node2_key"].Get<ID>();
    obj.SectionKey = doc["section_key"].Get<ID>();
}

void Truss::from_truss(const TrussDocument& doc, Load::PlaneNodeForce& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.PlaneNodeKey = doc["node_key"].Get<ID>();
    obj.YForce = doc["xforce"].GetOrDefault<Numeric>();
    obj.YForce = doc["yforce"].GetOrDefault<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Material::Elastic& obj)
{
    obj.Key = doc["key"].Get<ID>();

    obj.Density = doc["rho"].Get<Numeric>();
    obj.YoungsModules = doc["E"].Get<Numeric>();
    obj.PoissonRation = doc["pr"].Get<Numeric>();
}

void Truss::from_truss(const TrussDocument& doc, Section::Section_PlaneBar& obj)
{
    obj.Key = doc["key"].Get<ID>();
    obj.MaterialKey = doc["mat_key"].Get<ID>();
    obj.Area = doc["area"].Get<Numeric>();
}
