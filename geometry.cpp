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
}

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

void Geometry::constructCube()
{
    QVector3D verticesCube[] = {
        // front
        QVector3D(-1.0f, -1.0f, 1.0f),
        QVector3D(1.0f, -1.0f, 1.0f),
        QVector3D(1.0f, 1.0f, 1.0f),
        QVector3D(-1.0f, 1.0f, 1.0f),

        // back
        QVector3D(-1.0f, -1.0f, -1.0f),
        QVector3D(1.0f, -1.0f, -1.0f),
        QVector3D(1.0f, 1.0f, -1.0f),
        QVector3D(-1.0f, 1.0f, -1.0f),
    };

    GLushort indicesCube[] = {// front
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

    m_cubeVertexBuffer.create();
    m_cubeVertexBuffer.bind();
    m_cubeVertexBuffer.allocate(verticesCube, sizeof(verticesCube));

    m_cubeIndexBuffer.create();
    m_cubeIndexBuffer.bind();
    m_cubeIndexBuffer.allocate(indicesCube, sizeof(indicesCube));
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
