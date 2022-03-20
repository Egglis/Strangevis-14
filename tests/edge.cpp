#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../geometry/edge.h"

#include "doctest.h"

TEST_CASE("Constructed edge is independent of vertex order")
{
    QVector3D vertex1 {1,0,0};
    QVector3D vertex2 {0,1,0};
    Edge edge1{vertex1, vertex2};
    Edge edge2{vertex2, vertex1};
    REQUIRE(edge1.parameterization() == edge2.parameterization());
}

TEST_CASE("Parameterization  overload is consistent")
{
    QVector3D vertex1 {1,0,0};
    QVector3D vertex2 {0,1,0};
    Edge edge{vertex1, vertex2};
    REQUIRE(edge.parameterization(0.5) == edge.parameterization().first*0.5 + edge.parameterization().second);
}
