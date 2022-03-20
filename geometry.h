#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "geometry/cube.h"
#include "geometry/quad.h"

#include <QOpenGLBuffer>

class Geometry {
  protected:
    Geometry();

  public:
    static Geometry& instance();

    void bindQuad();
    void drawQuad();

    void bindCube();
    void drawCube();

  private:
    void allocateQuad();
    void allocateCube();

    QOpenGLBuffer m_quadVertexBuffer;
    QOpenGLBuffer m_quadIndexBuffer;

    QOpenGLBuffer m_cubeVertexBuffer;
    QOpenGLBuffer m_cubeIndexBuffer;
};
#endif // GEOMETRY_H
