#include "geometry.h"

#include <QDebug>

Geometry::Geometry()
    : m_quadVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_quadIndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_boxVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_boxIndexBuffer(QOpenGLBuffer::IndexBuffer)
{
    constructQuad();
    constructBox(QVector3D(1.0f, 1.0f, 1.0f));
}

void Geometry::constructQuad()
{
    QVector3D verticesQuad[] = {
        QVector3D(-1.0f, -1.0f, 0.0f), // bottom left corner
        QVector3D(-1.0f, 1.0f, 0.0f),  // top left corner
        QVector3D(1.0f, 1.0f, 0.0f),   // top right corner
        QVector3D(1.0f, -1.0f, 0.0f)   // bottom right corner
    };

    GLushort indicesQuad[] = {
        0, 1, 2, // first triangle (bottom left - top left - top right)
        0, 2, 3  // second triangle (bottom left - top right - bottom right)
    };

    m_quadVertexBuffer.create();
    m_quadVertexBuffer.bind();
    m_quadVertexBuffer.allocate(verticesQuad, sizeof(verticesQuad));

    m_quadIndexBuffer.create();
    m_quadIndexBuffer.bind();
    m_quadIndexBuffer.allocate(indicesQuad, sizeof(indicesQuad));
}

void Geometry::constructBox(QVector3D dimensions)
{
    auto [width, height, depth] = dimensions;
    float maxDim = std::max(width, std::max(height, depth));
    auto [x, y, z] = QVector3D{width / maxDim, height / maxDim, depth / maxDim};
    QVector3D verticesBox[] = {
        // front
        QVector3D(-x, -y, z),
        QVector3D(x, -y, z),
        QVector3D(x, y, z),
        QVector3D(-x, y, z),
        // back
        QVector3D(-x, -y, -z),
        QVector3D(x, -y, -z),
        QVector3D(x, y, -z),
        QVector3D(-x, y, -z),
    };

    if (m_boxVertexBuffer.isCreated())
    {
        m_boxVertexBuffer.destroy();
    }
    m_boxVertexBuffer.create();
    m_boxVertexBuffer.bind();
    m_boxVertexBuffer.allocate(verticesBox, sizeof(verticesBox));

    if (!m_boxIndexBuffer.isCreated())
    {
        GLushort indicesBox[] = {// front
                                  0, 1, 2, 2, 3, 0,
                                  // right
                                  1, 5, 6, 6, 2, 1,
                                  // back
                                  7, 6, 5, 5, 4, 7,
                                  // left
                                  4, 0, 3, 3, 7, 4,
                                  // bottom
                                  4, 5, 1, 1, 0, 4,
                                  // top
                                  3, 2, 6, 6, 7, 3};

        m_boxIndexBuffer.create();
        m_boxIndexBuffer.bind();
        m_boxIndexBuffer.allocate(indicesBox, sizeof(indicesBox));
    }
}

void Geometry::bindQuad()
{
    m_quadVertexBuffer.bind();
    m_quadIndexBuffer.bind();
}

void Geometry::drawQuad()
{
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Geometry::bindBox()
{
    m_boxVertexBuffer.bind();
    m_boxIndexBuffer.bind();
}

void Geometry::drawBox()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
}

Geometry& Geometry::instance()
{
    static Geometry geometry;
    return geometry;
}
