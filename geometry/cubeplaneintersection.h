#ifndef CUBEPLANEINTERSECTION_H
#define CUBEPLANEINTERSECTION_H
#include "cube.h"
#include "edge.h"
#include "plane.h"

#include <QMatrix4x4>
#include <QVector3D>


class CubePlaneIntersection
{
  public:
    CubePlaneIntersection(Plane plane);
    void changePlane(Plane plane);
    // Returns the intersections between a cube and a plane cutting the cube
    const std::vector<QVector3D>& getCubeIntersections() const
    {
        return m_cubeIntersections;
    };
    // Matrix that rotates the cube intersection points into the XY plane
    QMatrix4x4 getModelRotationMatrix() const { return m_modelRotationMatrix; };

    const std::vector<unsigned short>& getConvexHullIndexOrder() const
    {
        return m_sortedOrder;
    };

  private:
    void updateIntersections();
    Cube m_cube;
    std::vector<QVector3D> cubeIntersectionVertices();
    bool hasLinePlaneIntersection(Edge e);
    bool hasRayPlaneIntersection(QVector3D rayDirection, QVector3D rayOrigin);
    QVector3D linePlaneIntersectionPoint(Edge e) const;
    Plane m_plane;
    std::vector<QVector3D> m_cubeIntersections;
    QMatrix4x4 m_modelRotationMatrix;
    std::vector<unsigned short> m_sortedOrder;
};

#endif // CUBEPLANEINTERSECTION_H
