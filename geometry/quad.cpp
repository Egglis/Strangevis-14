#include "Quad.h"

Quad::Quad()
{
    m_vertices = {
        QVector3D(-1.0f, -1.0f, 0.0f), // bottom left corner
        QVector3D(-1.0f, 1.0f, 0.0f),  // top left corner
        QVector3D(1.0f, 1.0f, 0.0f),   // top right corner
        QVector3D(1.0f, -1.0f, 0.0f)   // bottom right corner
    };

    m_indices = {
        0, 1, 2, // first triangle (bottom left - top left - top right)
        0, 2, 3  // second triangle (bottom left - top right - bottom right)
    };
    constructEdges();
}

void Quad::constructEdges()
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
    // Should be 4 for a quad
    assert(m_edges.size() == 4);
}
