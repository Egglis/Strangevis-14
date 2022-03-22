#ifndef CUBEPLANEINTERSECTION_H
#define CUBEPLANEINTERSECTION_H
#include "geometry/plane.h"
#include "geometry/edge.h"
#include "geometry/cube.h"

#include <QVector2D>
#include <QVector3D>


class CubePlaneIntersection
{
  public:
    CubePlaneIntersection(Plane plane);
    void changePlane(Plane plane);
    const std::vector<QVector3D>& getTextureCoords() const
    {
        return m_textureCoords;
    };
    const std::vector<QVector2D>& getVertexPositions() const
    {
      return m_vertexPositions;
    };

  private:
    Cube m_cube;
    std::vector<QVector3D> intersectionVertices();
    bool hasLinePlaneIntersection(Edge e);
    bool hasRayPlaneIntersection(QVector3D rayDirection, QVector3D rayOrigin);
    QVector3D linePlaneIntersectionPoint(Edge e) const;
    Plane m_plane;
    std::vector<QVector3D> m_textureCoords;
    std::vector<QVector2D> m_vertexPositions;
    std::vector<QVector2D> rotateToXYPlane(std::vector<QVector3D> input);
};


#endif // CUBEPLANEINTERSECTION_H
