#include "plane.h"

#include <QMatrix4x4>


Plane::Plane(QVector4D planeEquation)
    : m_planeEquation{planeEquation}, m_planeNormal{m_planeEquation}
{
    m_planePoint = m_planeEquation.w() * m_planeNormal.normalized();
    assert(QVector3D::dotProduct(m_planePoint, m_planeNormal.normalized()) -
               m_planeEquation.w() <
           0.001);
}

Plane::Plane(QVector3D vertex0, QVector3D vertex1, QVector3D vertex2)
{
    m_planeNormal =
        QVector3D::crossProduct(vertex1 - vertex0, vertex2 - vertex1);
    m_planePoint = vertex0;
    m_planeEquation = derivePlaneEquation();
}

bool Plane::pointInPlane(QVector3D point) const
{
    return std::abs(QVector3D::dotProduct(point, m_planeNormal.normalized()) -
                    d()) < 0.0001;
}

void Plane::rotate(float angle, const QVector3D& vector)
{
    QMatrix4x4 rotMat{};
    rotMat.rotate(angle, vector);
    m_planeNormal = rotMat.map(m_planeNormal);
    m_planePoint = rotMat.map(m_planePoint);
    m_planeEquation = derivePlaneEquation();
}

void Plane::translate(const QVector3D& vector)
{
    m_planePoint += vector;
    m_planeEquation = derivePlaneEquation();
}

QVector4D Plane::derivePlaneEquation()
{
    QVector4D equation{
        m_planeNormal,
        QVector3D::dotProduct(m_planePoint, m_planeNormal.normalized())};
    assert(QVector3D::dotProduct(m_planePoint, m_planeNormal.normalized()) -
               equation.w() <
           0.001);
    return equation;
}
