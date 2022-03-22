#include "geometry.h"

#include <QDebug>
#include <QVector3D>

Geometry::Geometry()
    : m_quadVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_quadIndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_cubeVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_cubeIndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_sliceVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_sliceTextureCoordBuffer(QOpenGLBuffer::VertexBuffer),
      m_sliceIndexBuffer(QOpenGLBuffer::IndexBuffer)
{
    allocateQuad();
    allocateCube();
}

void Geometry::allocateQuad()
{
    Quad quad{};

    m_quadVertexBuffer.create();
    m_quadVertexBuffer.bind();
    m_quadVertexBuffer.allocate(quad.vertices().data(),
                                quad.vertices().size() *
                                    sizeof(quad.vertices()[0]));

    m_quadIndexBuffer.create();
    m_quadIndexBuffer.bind();
    m_quadIndexBuffer.allocate(quad.indices().data(),
                               quad.indices().size() *
                                   sizeof(quad.indices()[0]));
}

void Geometry::allocateCube()
{
    Cube cube{};

    m_cubeVertexBuffer.create();
    m_cubeVertexBuffer.bind();
    m_cubeVertexBuffer.allocate(cube.vertices().data(),
                                cube.vertices().size() *
                                    sizeof(cube.vertices()[0]));

    m_cubeIndexBuffer.create();
    m_cubeIndexBuffer.bind();
    m_cubeIndexBuffer.allocate(cube.indices().data(),
                               cube.indices().size() *
                                   sizeof(cube.indices()[0]));
}

void Geometry::allocateObliqueSlice(const CubePlaneIntersection& intersection)
{
    auto vertexPositions = intersection.getVertexPositions();
    auto textureCoords = intersection.getTextureCoords();
    auto sortedOrder = intersection.getConvexHullIndexOrder();
    m_sliceIndices = sortedOrder.size();

    if (m_sliceVertexBuffer.isCreated())
        m_sliceVertexBuffer.destroy();
    m_sliceVertexBuffer.create();
    m_sliceVertexBuffer.bind();
    m_sliceVertexBuffer.allocate(vertexPositions.data(),
                                 vertexPositions.size() *
                                     sizeof(vertexPositions[0]));

    if (m_sliceTextureCoordBuffer.isCreated())
        m_sliceTextureCoordBuffer.destroy();
    m_sliceTextureCoordBuffer.create();
    m_sliceTextureCoordBuffer.bind();
    m_sliceTextureCoordBuffer.allocate(
        textureCoords.data(), textureCoords.size() * sizeof(textureCoords[0]));

    if (m_sliceIndexBuffer.isCreated())
        m_sliceIndexBuffer.destroy();
    m_sliceIndexBuffer.create();
    m_sliceIndexBuffer.bind();
    m_sliceIndexBuffer.allocate(sortedOrder.data(),
                                sortedOrder.size() * sizeof(sortedOrder[0]));
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

void Geometry::bindObliqueSliceVertex()
{
    m_sliceVertexBuffer.bind();
    m_sliceIndexBuffer.bind();
}

void Geometry::bindObliqueSliceTexCoord()
{
    m_sliceTextureCoordBuffer.bind();
    m_sliceIndexBuffer.bind();
}

void Geometry::drawObliqueSlice()
{
    glDrawElements(GL_TRIANGLE_FAN, m_sliceIndices, GL_UNSIGNED_SHORT, nullptr);
}

Geometry& Geometry::instance()
{
    static Geometry geometry;
    return geometry;
}
