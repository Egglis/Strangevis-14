#ifndef CUBEPLANEINTERSECTION_H
#define CUBEPLANEINTERSECTION_H
#include "box.h"
#include "cube.h"
#include "edge.h"
#include "plane.h"

#include <QVector2D>
#include <QVector3D>

class CubePlaneIntersection
{
  public:
    CubePlaneIntersection(Plane plane, Box box);
    void changePlane(Plane plane);
    const std::vector<QVector3D>& getTextureCoords()
    {
        updateIfNeeded();
        return m_textureCoords;
    };
    const std::vector<QVector2D>& getVertexPositions()
    {
        updateIfNeeded();
        return m_vertexPositions;
    };

    const std::vector<unsigned short>& getConvexHullIndexOrder()
    {
        updateIfNeeded();
        return m_sortedOrder;
    };
    void changeScaling(QVector3D dims);

  private:
    void updateIfNeeded();
    Box m_box;
    std::vector<QVector3D> textureIntersectionVertices();
    std::vector<QVector3D> vertexIntersectionVertices();
    bool hasLinePlaneIntersection(Edge e);
    bool hasRayPlaneIntersection(QVector3D rayDirection, QVector3D rayOrigin);
    QVector3D linePlaneIntersectionPoint(Edge e) const;
    Plane m_plane;
    std::vector<QVector3D> m_textureCoords;
    std::vector<QVector2D> m_vertexPositions;
    std::vector<unsigned short> m_sortedOrder;
    bool m_updateNeeded;
};

#endif // CUBEPLANEINTERSECTION_H
