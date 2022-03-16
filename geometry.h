#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QOpenGLBuffer>
#include <QVector3D>
class Geometry {
  protected:
    Geometry();

  public:
    static Geometry& instance();

    void bindQuad();
    void drawQuad();

    void bindBox();
    void drawBox();

    void constructBox(QVector3D dimensions);

  private:
    void constructQuad();

    QOpenGLBuffer m_quadVertexBuffer;
    QOpenGLBuffer m_quadIndexBuffer;

    QOpenGLBuffer m_boxVertexBuffer;
    QOpenGLBuffer m_boxIndexBuffer;
};
#endif // GEOMETRY_H
