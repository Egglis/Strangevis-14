#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "geometry/cube.h"
#include "geometry/cubeplaneintersection.h"
#include "geometry/quad.h"

#include <QOpenGLBuffer>

class Geometry
{
  protected:
    Geometry();

  public:
    static Geometry& instance();

    void bindQuad();
    void drawQuad();

    void bindCube();
    void drawCube();

    void bindObliqueSliceIntersectionCoords();
    void drawObliqueSlice();

    void allocateObliqueSlice(CubePlaneIntersection& intersection);

    void allocateLightSource();
    void bindLightSource();
    void drawLightSource();

  private:
    void allocateQuad();
    void allocateCube();

    GLsizei m_sliceIndices;
    QOpenGLBuffer m_sliceCubeIntersectionCoordBuffer;
    QOpenGLBuffer m_sliceIndexBuffer;

    QOpenGLBuffer m_quadVertexBuffer;
    QOpenGLBuffer m_quadIndexBuffer;

    QOpenGLBuffer m_cubeVertexBuffer;
    QOpenGLBuffer m_cubeIndexBuffer;

    QOpenGLBuffer m_lightVertexBuffer;
    QOpenGLBuffer m_lightIndexBuffer;
};
#endif // GEOMETRY_H
