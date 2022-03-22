#include "geometry.h"

#include <QDebug>
#include <QVector3D>

Geometry::Geometry()
    : m_quadVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_quadIndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_cubeVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_cubeIndexBuffer(QOpenGLBuffer::IndexBuffer)
{
    allocateQuad();
    allocateCube();
}

void Geometry::allocateQuad()
{
    Quad quad{};

    m_quadVertexBuffer.create();
    m_quadVertexBuffer.bind();
    m_quadVertexBuffer.allocate(quad.vertices().data(), sizeof(quad.vertices()[0]));

    m_quadIndexBuffer.create();
    m_quadIndexBuffer.bind();
    m_quadIndexBuffer.allocate(quad.indices().data(), sizeof(quad.indices()[0]));
}

void Geometry::allocateCube()
{
    Cube cube{};

    m_cubeVertexBuffer.create();
    m_cubeVertexBuffer.bind();
    m_cubeVertexBuffer.allocate(cube.vertices().data(), cube.vertices().size()*sizeof(cube.vertices()[0]));

    m_cubeIndexBuffer.create();
    m_cubeIndexBuffer.bind();
    m_cubeIndexBuffer.allocate(cube.indices().data(), cube.indices().size()* sizeof(cube.indices()[0]));
};

void Geometry::bindQuad()
{
    m_quadVertexBuffer.bind();
    m_quadIndexBuffer.bind();
}

void Geometry::drawQuad()
{
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Geometry::bindCube()
{
    m_cubeVertexBuffer.bind();
    m_cubeIndexBuffer.bind();
}

void Geometry::drawCube()
{
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
}

Geometry& Geometry::instance()
{
    static Geometry geometry;
    return geometry;
}
