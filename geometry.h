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
    void releaseObliqueSliceIntersectionCoords();
    void drawObliqueSlice();

    void bindLightSource();
    void drawLightSource();

    void allocateObliqueSlice(CubePlaneIntersection& intersection);

  private:
    void allocateQuad();
    void allocateCube();
    void allocateLightSource();

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
