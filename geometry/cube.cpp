#include "cube.h"

Cube::Cube()
{
    m_vertices = {
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

    m_indices = {// front
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

    constructEdges();
}

void Cube::constructEdges()
{
    for (int i = 0; i < m_vertices.size() - 1; i++)
    {
        for (int j = i + 1; j < m_vertices.size(); j++)
        {
            // Create edges between all nearest neighbours
            if ((m_vertices[i] - m_vertices[j]).lengthSquared() == 4.0f)
            {
                m_edges.push_back(Edge(m_vertices[i], m_vertices[j]));
            }
        }
    }
    // Should be 12 for a cube
    assert(m_edges.size() == 12);
}
