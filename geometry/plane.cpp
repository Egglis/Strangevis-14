#include "plane.h"

Plane::Plane(QVector4D planeEquation) : m_planeEquation{planeEquation}
{
    QVector3D unNormalizedPlaneNormal{m_planeEquation.x(), m_planeEquation.y(),
                                      m_planeEquation.z()};
    m_planeNormal = unNormalizedPlaneNormal.normalized();
    m_planeEquation = QVector4D(
        m_planeNormal, m_planeEquation.w() / unNormalizedPlaneNormal.length());
    m_planePoint = m_planeEquation.w() * m_planeNormal;
    assert(QVector3D::dotProduct(m_planePoint, m_planeNormal) -
               m_planeEquation.w() <
           0.001);
}

Plane::Plane(QVector3D vertex0, QVector3D vertex1, QVector3D vertex2)
    : m_planeEquation{0, 0, 1, 0}
{
    m_planeNormal =
        QVector3D::crossProduct(vertex1 - vertex0, vertex2 - vertex1)
            .normalized();
    m_planePoint = vertex0;
    m_planeEquation = QVector4D(
        m_planeNormal, QVector3D::dotProduct(m_planePoint, m_planeNormal));
    assert(QVector3D::dotProduct(m_planePoint, m_planeNormal) -
               m_planeEquation.w() <
           0.001);
}
