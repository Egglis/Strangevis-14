#ifndef PLANE_H
#define PLANE_H

#include <QVector3D>
#include <QVector4D>

class Plane
{
  public:
    Plane(QVector4D planeEquation);
    Plane(QVector3D vertex0, QVector3D vertex1, QVector3D vertex2);

    QVector3D normal() const { return m_planeNormal; };
    QVector3D point() const { return m_planePoint; };
    QVector4D equation() const { return m_planeEquation; };
    float d() const { return m_planeEquation.w(); };

  private:
    QVector4D m_planeEquation;
    QVector3D m_planeNormal;
    QVector3D m_planePoint;
};

#endif // PLANE_H
