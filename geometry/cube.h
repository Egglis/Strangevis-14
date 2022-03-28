#ifndef CUBE_H
#define CUBE_H

#include "edge.h"

#include <QVector3D>

class Cube
{
  public:
    Cube();
    constexpr const std::vector<QVector3D>& vertices() const
    {
        return m_vertices;
    };
    constexpr const std::vector<unsigned short>& indices() const
    {
        return m_indices;
    };
    const std::vector<Edge>& edges() const { return m_edges; };

  protected:
    std::vector<Edge>& protectedEdges() { return m_edges; };

  private:
    void constructEdges();
    std::vector<QVector3D> m_vertices;
    std::vector<unsigned short> m_indices;
    std::vector<Edge> m_edges;
};

#endif // CUBE_H
