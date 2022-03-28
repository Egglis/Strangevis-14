#ifndef BOX_H
#define BOX_H

#include "cube.h"

#include <QVector3D>

class Box
{
  public:
    Box();
    Box(QVector3D dims);
    const QVector3D& dims() const { return m_dims; };
    const Cube& cube() const { return m_cube; };
    const std::vector<Edge>& edges() const { return m_edges; };

  private:
    QVector3D m_dims;
    std::vector<Edge> m_edges;
    Cube m_cube;
};

#endif // BOX_H
