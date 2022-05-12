#include "geometry.h"

#include <QDebug>
#include <QVector3D>

Geometry::Geometry()
    : m_quadVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_quadIndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_cubeVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_cubeIndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_sliceCubeIntersectionCoordBuffer(QOpenGLBuffer::VertexBuffer),
      m_sliceIndexBuffer(QOpenGLBuffer::IndexBuffer),
      m_lightVertexBuffer(QOpenGLBuffer::VertexBuffer),
      m_lightIndexBuffer(QOpenGLBuffer::IndexBuffer)
{
    allocateQuad();
    allocateCube();
    allocateLightSource();
}

void Geometry::allocateQuad()
{
    Quad quad{};

    m_quadVertexBuffer.create();
    m_quadVertexBuffer.bind();
    m_quadVertexBuffer.allocate(
        quad.vertices().data(),
        static_cast<int>(quad.vertices().size() * sizeof(quad.vertices()[0])));

    m_quadIndexBuffer.create();
    m_quadIndexBuffer.bind();
    m_quadIndexBuffer.allocate(
        quad.indices().data(),
        static_cast<int>(quad.indices().size() * sizeof(quad.indices()[0])));
}

void Geometry::allocateCube()
{
    Cube cube{};

    m_cubeVertexBuffer.create();
    m_cubeVertexBuffer.bind();
    m_cubeVertexBuffer.allocate(
        cube.vertices().data(),
        static_cast<int>(cube.vertices().size() * sizeof(cube.vertices()[0])));

    m_cubeIndexBuffer.create();
    m_cubeIndexBuffer.bind();
    m_cubeIndexBuffer.allocate(
        cube.indices().data(),
        static_cast<int>(cube.indices().size() * sizeof(cube.indices()[0])));
}

void Geometry::allocateObliqueSlice(CubePlaneIntersection& intersection)
{
    auto cubeIntersectionCoords = intersection.getCubeIntersections();
    auto sortedOrder = intersection.getConvexHullIndexOrder();
    m_sliceIndices = static_cast<GLsizei>(sortedOrder.size());

    if (m_sliceCubeIntersectionCoordBuffer.isCreated())
        m_sliceCubeIntersectionCoordBuffer.destroy();
    m_sliceCubeIntersectionCoordBuffer.create();
    m_sliceCubeIntersectionCoordBuffer.bind();
    m_sliceCubeIntersectionCoordBuffer.allocate(
        cubeIntersectionCoords.data(),
        static_cast<int>(cubeIntersectionCoords.size() *
                         sizeof(cubeIntersectionCoords[0])));
    m_sliceCubeIntersectionCoordBuffer.release();

    if (m_sliceIndexBuffer.isCreated())
        m_sliceIndexBuffer.destroy();
    m_sliceIndexBuffer.create();
    m_sliceIndexBuffer.bind();
    m_sliceIndexBuffer.allocate(
        sortedOrder.data(),
        static_cast<int>(sortedOrder.size() * sizeof(sortedOrder[0])));
    m_sliceIndexBuffer.release();
}

void Geometry::allocateLightSource()
{
    m_lightVertexBuffer.create();
    m_lightVertexBuffer.bind();
    m_lightVertexBuffer.allocate(sizeof(QVector3D));

    m_lightIndexBuffer.create();
    m_lightIndexBuffer.bind();
    m_lightIndexBuffer.allocate(sizeof(QVector3D));
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

void Geometry::bindObliqueSliceIntersectionCoords()
{
    m_sliceCubeIntersectionCoordBuffer.bind();
    m_sliceIndexBuffer.bind();
}

void Geometry::releaseObliqueSliceIntersectionCoords()
{
    m_sliceCubeIntersectionCoordBuffer.release();
    m_sliceIndexBuffer.release();
}

void Geometry::drawObliqueSlice()
{
    glDrawElements(GL_TRIANGLE_FAN, m_sliceIndices, GL_UNSIGNED_SHORT, nullptr);
}

void Geometry::bindLightSource()
{
    m_lightVertexBuffer.bind();
    m_lightIndexBuffer.bind();
}

void Geometry::drawLightSource()
{
    glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, nullptr);
}

Geometry& Geometry::instance()
{
    static Geometry geometry;
    return geometry;
}
