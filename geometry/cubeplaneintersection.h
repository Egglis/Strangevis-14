#ifndef CUBEPLANEINTERSECTION_H
#define CUBEPLANEINTERSECTION_H
#include "cube.h"
#include "edge.h"
#include "plane.h"

#include <QMatrix4x4>
#include <QVector3D>
#include <unordered_set>
#include <QSet>

enum class Intersection {
    NONE,
    SINGLE,
    PARALLEL
};

class CubePlaneIntersection
{
  public:
    CubePlaneIntersection(Plane plane);
    void changePlane(Plane plane);
    // Returns the intersections between a cube and a plane cutting the cube
    std::vector<QVector3D> getCubeIntersections() const
    {
        return std::vector<QVector3D>{m_cubeIntersections.begin(), m_cubeIntersections.end()};
    };
    // Matrix that rotates the cube intersection points into the XY plane
    QMatrix4x4 getModelRotationMatrix() const { return m_modelRotationMatrix; };

    const std::vector<unsigned short>& getConvexHullIndexOrder() const
    {
        return m_sortedOrder;
    };
    const Plane& plane() const {return m_plane;};

  private:
    void updateIntersections();
    Cube m_cube;
    QSet<QVector3D> cubeIntersectionVertices();
    Intersection hasLinePlaneIntersection(Edge e) const;
    bool hasRayPlaneIntersection(QVector3D rayDirection, QVector3D rayOrigin) const;
    bool hasEdgeParallelToPlane(Edge e) const;
    QVector3D linePlaneIntersectionPoint(Edge e) const;
    Plane m_plane;
    QSet<QVector3D> m_cubeIntersections;
    QMatrix4x4 m_modelRotationMatrix;
    std::vector<unsigned short> m_sortedOrder;
};

#endif // CUBEPLANEINTERSECTION_H
