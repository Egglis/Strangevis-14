#ifndef EDGE_H
#define EDGE_H

#include <QVector3D>

class Edge
{
  public:
    Edge(QVector3D a, QVector3D b);

    QVector3D parameterization(float t) const;
    // Returns the parameterization of the line segment with t==0.0 at starting
    // edge and t==1.0 at ending edge
    std::pair<QVector3D, QVector3D> parameterization() const;
    QVector3D direction() const;
    QVector3D start() const;
    QVector3D end() const;
    void stretch(float stretch);
    void scale(QVector3D scale);

  private:
    QVector3D m_a;
    QVector3D m_b;
};

bool edgeLessThan(QVector3D a, QVector3D b);

#endif // EDGE_H
