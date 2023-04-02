#include <array>
#include <iostream>
#include <vector>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <Truss/Serializer/Serializers.hpp>
#include <Truss/Utils/GaussianQuadrature.hpp>
#include <TrussDocument/TrussDocument.hpp>
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

    SECTION("Truss::Constraint::NodeSetConstraint")
    {
        Constraint::NodeSetConstraint original;
        original.Key = 123;
        original.XDisplacement = true;
        original.YDisplacement = false;
        original.ZDisplacement = true;
        original.XRotation = false;
        original.YRotation = true;
        original.ZRotation = false;
        original.NodeSetKey = 456;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Constraint::NodeSetConstraint>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.XDisplacement == original.XDisplacement);
        REQUIRE(result.YDisplacement == original.YDisplacement);
        REQUIRE(result.ZDisplacement == original.ZDisplacement);
        REQUIRE(result.XRotation == original.XRotation);
        REQUIRE(result.YRotation == original.YRotation);
        REQUIRE(result.ZRotation == original.ZRotation);
        REQUIRE(result.NodeSetKey == original.NodeSetKey);
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

    SECTION("Truss::Load::NodeSetLoad")
    {
        Load::NodeSetLoad original;
        original.Key = 123;
        original.NodeSetKey = 456;
        original.XForce = 1.0f;
        original.YForce = 2.0f;
        original.ZForce = 3.0f;
        original.XMoment = 4.0f;
        original.YMoment = 5.0f;
        original.ZMoment = 6.0f;
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Load::NodeSetLoad>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.NodeSetKey == original.NodeSetKey);
        REQUIRE(result.XForce == original.XForce);
        REQUIRE(result.YForce == original.YForce);
        REQUIRE(result.ZForce == original.ZForce);
        REQUIRE(result.XMoment == original.XMoment);
        REQUIRE(result.YMoment == original.YMoment);
        REQUIRE(result.ZMoment == original.ZMoment);
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
        original.NodeKeys = {1, 2, 3, 4, 5};
        TrussDocument doc = TrussDocument(original);
        auto result = doc.Get<Set::NodeSet>();
        REQUIRE(result.Key == original.Key);
        REQUIRE(result.NodeKeys == original.NodeKeys);
    }
}


TEST_CASE("Utils", "[Utils]")
{
    SECTION("GetGaussWeightsAndPoints2D(GaussianPoint2D::One)")
    {
        auto [weight, points] = GetGaussWeightsAndPoints2D<float>(GaussianPoint2D::One);
        REQUIRE(weight.size() == 1);
        REQUIRE(points.size() == 2);
        REQUIRE(weight(0) == Catch::Approx(4.0f));
        REQUIRE(points(0) == Catch::Approx(0.0f));
        REQUIRE(points(1) == Catch::Approx(0.0f));
    }

    SECTION("GetGaussWeightsAndPoints2D(GaussianPoint2D::Four)")
    {
        auto [weight, points] = GetGaussWeightsAndPoints2D<float>(GaussianPoint2D::Four);
        REQUIRE(weight.size() == 4);
        REQUIRE(points.size() == 8);
        REQUIRE(weight(0) == Catch::Approx(1.0f));
        REQUIRE(weight(1) == Catch::Approx(1.0f));
        REQUIRE(weight(2) == Catch::Approx(1.0f));
        REQUIRE(weight(3) == Catch::Approx(1.0f));
        REQUIRE(points(0, 0) == Catch::Approx(-0.577350269189626f));
        REQUIRE(points(0, 1) == Catch::Approx(-0.577350269189626f));
        REQUIRE(points(1, 0) == Catch::Approx(-0.577350269189626f));
        REQUIRE(points(1, 1) == Catch::Approx(0.577350269189626f));
        REQUIRE(points(2, 0) == Catch::Approx(0.577350269189626f));
        REQUIRE(points(2, 1) == Catch::Approx(-0.577350269189626f));
        REQUIRE(points(3, 0) == Catch::Approx(0.577350269189626f));
        REQUIRE(points(3, 1) == Catch::Approx(0.577350269189626f));
    }

    SECTION("GaussianQuadrature_1")
    {
        IntegrateFunc<float> func = [](const VectorX<float>& x, const VectorX<float>&) -> VectorX<float> {
            return VectorX<float>::Ones(x.size());
        };
        Eigen::Matrix<float, 4, 2> vertices;
        vertices << 0, 0, 5, -1, 4, 5, 1, 4;
        auto result1 = GaussianQuadrature2D(func, vertices, GaussianPoint2D::One);
        REQUIRE(result1 == Catch::Approx(20.0f));
        auto result2 = GaussianQuadrature2D(func, vertices, GaussianPoint2D::Four);
        REQUIRE(result2 == Catch::Approx(20.0f));
    }

    SECTION("GaussianQuadrature_2")
    {
        IntegrateFunc<float> func = [](const VectorX<float>& x, const VectorX<float>&) -> VectorX<float> {
            return VectorX<float>::Ones(x.size());
        };
        Eigen::Matrix<float, 4, 2> vertices;
        vertices << -1, -1, 5, -3, 10, 5, 7, 9;
        auto result1 = GaussianQuadrature2D(func, vertices, GaussianPoint2D::One);
        REQUIRE(result1 == Catch::Approx(60.0f));
        auto result2 = GaussianQuadrature2D(func, vertices, GaussianPoint2D::Four);
        REQUIRE(result2 == Catch::Approx(60.0f));
    }

    SECTION("GaussianQuadrature_3")
    {
        using namespace std;
        IntegrateFunc<float, Matrix3x3<float>> func = [](const VectorX<float>& x, const VectorX<float>&) -> VectorX<Matrix3x3<float>> {
            VectorX<Matrix3x3<float>> result;
            result.resize(x.size());
            for (int i = 0; i < x.size(); i++)
            {
                result(i) = Matrix3x3<float>::Ones();
            }
            return result;
        };
        Eigen::Matrix<float, 4, 2> vertices;
        vertices << 0, 0, 5, -1, 4, 5, 1, 4;
        auto result1 = GaussianQuadrature2D(func, vertices, GaussianPoint2D::One);
        // requires all element of result1 is 20
        for (float element: Eigen::Vector<float, 9>(result1.reshaped(9, 1)))
        {
            REQUIRE(element == Catch::Approx(20.0f));
        }
    }
}