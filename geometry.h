#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "geometry/cubeplaneintersection.h"
#include "geometry/cube.h"
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

    void bindObliqueSliceVertex();
    void bindObliqueSliceTexCoord();
    void drawObliqueSlice();

    void allocateObliqueSlice(CubePlaneIntersection& intersection);

  private:
    void allocateQuad();
    void allocateCube();

    unsigned short m_sliceIndices;
    QOpenGLBuffer m_sliceVertexBuffer;
    QOpenGLBuffer m_sliceTextureCoordBuffer;
    QOpenGLBuffer m_sliceIndexBuffer;

    QOpenGLBuffer m_quadVertexBuffer;
    QOpenGLBuffer m_quadIndexBuffer;

    QOpenGLBuffer m_cubeVertexBuffer;
    QOpenGLBuffer m_cubeIndexBuffer;
};
#endif // GEOMETRY_H
