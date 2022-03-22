#include "edge.h"

Edge::Edge(QVector3D a, QVector3D b) : m_a{a}, m_b{b}
{
    // Use smallest x vertex in first slot. If equal, use smallest y. If equal,
    // use smallest z.
    if (a.x() == b.x())
    {
        if (a.y() == b.y())
        {
            if (a.z() > b.z())
            {
                std::swap(m_a, m_b);
            }
        }
        else if (a.y() > b.y())
        {
            std::swap(m_a, m_b);
        }
    }
    else if (a.x() > b.x())
    {
        std::swap(m_a, m_b);
    }
}

QVector3D Edge::parameterization(float t) const
{
    return m_b * t + (1 - t) * m_a;
}

std::pair<QVector3D, QVector3D> Edge::parameterization() const
{
    QVector3D first = m_b - m_a;
    QVector3D second = start();
    return std::pair(first, second);
}

// Not normalized
QVector3D Edge::direction() const { return m_b - m_a; }

QVector3D Edge::start() const { return m_a; }
QVector3D Edge::end() const { return m_b; }
