#include "box.h"

Box::Box() : m_dims{1, 1, 1} {}
Box::Box(QVector3D dims) : m_dims{dims}, m_cube{}
{
    float minDim = std::min(dims.x(), std::min(dims.y(), dims.z()));
    for (const auto& edge : m_cube.edges())
    {
        m_edges.push_back(edge);
        if (minDim != 0)
            m_edges.back().scale(dims / minDim);
    }
}
