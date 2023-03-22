#include <Truss/Serializer/Serializers.hpp>
#include <TrussDocument/TrussDocument.hpp>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <vector>
using namespace Truss;

TEST_CASE("Test (De)Serializer", "[de_serializer]")
{
    SECTION("Truss::Node")
    {
        Node original;
        original.Key = 123;
        original.X = 1.0f;
        original.Y = 2.0f;
        original.Z = 3.0f;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Node>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.X == original.X);
        REQUIRE(result.Y == original.Y);
        REQUIRE(result.Z == original.Z);
    }

    SECTION("Truss::Constraint::NodeConstraint")
    {
        Constraint::NodeConstraint original;
        original.Key = 123;
        original.XDisplacement = true;
        original.YDisplacement = false;
        original.ZDisplacement = true;
        original.XRotation = false;
        original.YRotation = true;
        original.ZRotation = false;
        original.NodeKey = 456;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Constraint::NodeConstraint>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.XDisplacement == original.XDisplacement);
        REQUIRE(result.YDisplacement == original.YDisplacement);
        REQUIRE(result.ZDisplacement == original.ZDisplacement);
        REQUIRE(result.XRotation == original.XRotation);
        REQUIRE(result.YRotation == original.YRotation);
        REQUIRE(result.ZRotation == original.ZRotation);
        REQUIRE(result.NodeKey == original.NodeKey);
    }

    SECTION("Truss::Element::Bar")
    {
        Element::Bar original;
        original.Key = 123;
        original.LeftNodeKey = 456;
        original.RightNodeKey = 789;
        original.SectionKey = 101112;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Element::Bar>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.LeftNodeKey == original.LeftNodeKey);
        REQUIRE(result.RightNodeKey == original.RightNodeKey);
        REQUIRE(result.SectionKey == original.SectionKey);
    }

    SECTION("Truss::Element::Beam")
    {
        Element::Beam original;
        original.Key = 123;
        original.LeftNodeKey = 456;
        original.RightNodeKey = 789;
        original.SectionKey = 101112;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Element::Beam>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.LeftNodeKey == original.LeftNodeKey);
        REQUIRE(result.RightNodeKey == original.RightNodeKey);
        REQUIRE(result.SectionKey == original.SectionKey);
    }

    SECTION("Truss::Element::CSTriangle")
    {
        Element::CSTriangle original;
        original.Key = 123;
        original.LeftNodeKey = 456;
        original.RightNodeKey = 789;
        original.TopNodeKey = 101112;
        original.SectionKey = 131415;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Element::CSTriangle>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.LeftNodeKey == original.LeftNodeKey);
        REQUIRE(result.RightNodeKey == original.RightNodeKey);
        REQUIRE(result.TopNodeKey == original.TopNodeKey);
        REQUIRE(result.SectionKey == original.SectionKey);
    }

    SECTION("Truss::Load::NodeLoad")
    {
        Load::NodeLoad original;
        original.Key = 123;
        original.NodeKey = 456;
        original.XForce = 1.0f;
        original.YForce = 2.0f;
        original.ZForce = 3.0f;
        original.XMoment = 4.0f;
        original.YMoment = 5.0f;
        original.ZMoment = 6.0f;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Load::NodeLoad>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.NodeKey == original.NodeKey);
        REQUIRE(result.XForce == original.XForce);
        REQUIRE(result.YForce == original.YForce);
        REQUIRE(result.ZForce == original.ZForce);
        REQUIRE(result.XMoment == original.XMoment);
        REQUIRE(result.YMoment == original.YMoment);
        REQUIRE(result.ZMoment == original.ZMoment);
    }

    SECTION("Truss::Load::BeamUniformLoad")
    {
        Load::BeamUniformLoad original;
        original.Key = 123;
        original.BeamKey = 456;
        original.YForce = 1.0f;
        original.ZForce = 2.0f;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Load::BeamUniformLoad>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.BeamKey == original.BeamKey);
        REQUIRE(result.YForce == original.YForce);
        REQUIRE(result.ZForce == original.ZForce);
    }

    SECTION("Truss::Material::Elastic")
    {
        Material::Elastic original;
        original.Key = 123;
        original.YoungsModules = 1.0f;
        original.PoissonRation = 2.0f;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Material::Elastic>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.YoungsModules == original.YoungsModules);
        REQUIRE(result.PoissonRation == original.PoissonRation);
    }

    SECTION("Truss::Set::NodeSet")
    {
        Set::NodeSet original;
        original.Key = 123;
        original.NodeKeys = { 1, 2, 3, 4, 5 };
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Set::NodeSet>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.NodeKeys == original.NodeKeys);
    }


}