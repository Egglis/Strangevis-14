#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../geometry/edge.h"
#include "../geometry/quad.h"
#include "../geometry/cube.h"
#include "../geometry/plane.h"
#include "../geometry/cubeplaneintersection.h"

#include "doctest.h"

TEST_CASE("Constructed edge is independent of vertex order")
{
    QVector3D vertex1 {1,0,0};
    QVector3D vertex2 {0,1,0};
    Edge edge1{vertex1, vertex2};
    Edge edge2{vertex2, vertex1};
    REQUIRE(edge1.parameterization() == edge2.parameterization());
}

TEST_CASE("Parameterization overload is consistent")
{
    QVector3D vertex1 {1,0,0};
    QVector3D vertex2 {0,1,0};
    Edge edge{vertex1, vertex2};
    REQUIRE(edge.parameterization(0.5) == edge.parameterization().first*0.5 + edge.parameterization().second);
}

TEST_CASE("Quads have 4 edges")
{
    Quad quad{};
    REQUIRE(quad.edges().size() == 4);
}

TEST_CASE("Quads have 4 vertices")
{
    Quad quad{};
    REQUIRE(quad.vertices().size() == 4);
}

TEST_CASE("Neighboring Quad edges are orthogonal")
{
    Quad quad{};
    auto vertices = quad.vertices();
    auto edges = quad.edges();
    for (const auto& vertex : vertices)
    {
        std::vector<Edge> neighboringEdges = edges;
        neighboringEdges.erase(std::remove_if(neighboringEdges.begin(), neighboringEdges.end(), [&vertex](const auto& edge)
        {
            return (edge.start() != vertex && edge.end() != vertex);
        }), neighboringEdges.end());
        REQUIRE(neighboringEdges.size() == 2);
        REQUIRE(std::abs(QVector3D::dotProduct(neighboringEdges[0].direction(), neighboringEdges[1].direction())) < 0.001);

    }
}

TEST_CASE("Cubes have 12 edges")
{
    Cube cube{};
    REQUIRE(cube.edges().size() == 12);
}

TEST_CASE("Cubes have 8 vertices")
{
    Cube cube{};
    REQUIRE(cube.vertices().size() == 8);
}
TEST_CASE("Neighboring Cube edges are orthogonal")
{
    Cube cube{};
    auto vertices = cube.vertices();
    auto edges = cube.edges();
    for (const auto& vertex : vertices)
    {
        std::vector<Edge> neighboringEdges = edges;
        neighboringEdges.erase(std::remove_if(neighboringEdges.begin(), neighboringEdges.end(), [&vertex](const auto& edge)
        {
            return (edge.start() != vertex && edge.end() != vertex);
        }), neighboringEdges.end());
        REQUIRE(neighboringEdges.size() == 3);
        CHECK(std::abs(QVector3D::dotProduct(neighboringEdges[0].direction(), neighboringEdges[1].direction())) < 0.001);
        CHECK(std::abs(QVector3D::dotProduct(neighboringEdges[0].direction(), neighboringEdges[2].direction())) < 0.001);
        CHECK(std::abs(QVector3D::dotProduct(neighboringEdges[1].direction(), neighboringEdges[2].direction())) < 0.001);

    }
}

TEST_CASE("Plane point and normal obey the plane equation")
{
    Plane plane0{QVector4D(0.0,0.0,1.0,0.0)};
    Plane plane1{QVector3D(0.0,0.0,0.0), QVector3D(0.0,1.0,0.0), QVector3D(0.0,0.0,1.0)};
    CHECK(std::abs(QVector3D::dotProduct(plane0.normal(), plane0.point())) < 0.001);
    CHECK(std::abs(QVector3D::dotProduct(plane1.normal(), plane1.point())) < 0.001);
}

TEST_CASE("Cube-plane Intersection produces correct number of vertices")
{
    Plane threeVertices(QVector4D(0.9, 0.9, 0.9, 1.5));  // Shaves a corner
    CubePlaneIntersection interThree{threeVertices, Box{}};
    CHECK(interThree.getVertexPositions().size() == 3);

    Plane fourVertices(QVector4D(0, 0, 1, 0));  // Cuts it in half
    CubePlaneIntersection interFour{fourVertices, Box{}};
    CHECK(interFour.getVertexPositions().size() == 4);

    // TODO: Add cases for 5 and 6 vertices.
}
