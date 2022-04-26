#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include <QList>
#include <QOpenGLFunctions>
#include <QPointF>
#include <QString>

namespace tfn
{
namespace points
{
constexpr static QPointF START_POINT = QPointF(0, 0);
constexpr static QPointF END_POINT = QPointF(1, 1);
}; // namespace points
namespace nodes {
  constexpr static QPointF OFFSET = QPointF(0.06, 0.06);
  constexpr static int RIGHT = 0;
  constexpr static int TOP = 1;
}
// TODO change to horizontal / vertical instead of RIGHT and TOP!!


class ControlPoint : public QPointF
{
  public:
    ControlPoint(QPointF position);
    void setControlNode(int index, QPointF pos);
    void setAllControlNodes(QList<QPointF> nodes);
    QList<QPointF> getControlNodes() {return m_controlNodes; };
  private:
    QPointF clampToDirections(int dir, QPointF pos);
    QList<QPointF> m_controlNodes;   
};

class TransferFunction
{
  public:
    TransferFunction();
    bool addControlPoint(ControlPoint cp);
    bool removeControlPoint(ControlPoint point);
    std::vector<GLfloat> applyTransferFunction(const std::vector<GLfloat> cmap);
    void interpolatePoints();

    QList<ControlPoint> getControlPoints() { return m_controlPoints; };
    int indexOf(ControlPoint point) { return m_controlPoints.indexOf(point); };
    void replace(int index, ControlPoint point);
    void reset();


    QPointF deCasteljau(QPointF p0, QPointF p1, QPointF p2, QPointF p3, float t);
    void setControlNodePos(int index, int dir, QPointF pos);

    QList<QPointF> getInterpolatedPoints() {return m_interpolatedPoints;} ;

  private:
    constexpr static float getPt(float n1, float n2, float perc);
    QPointF clampToNeighbours(int index, ControlPoint point);
    QPointF clampToDomain(ControlPoint point, QPointF max, QPointF min);

    QList<ControlPoint> m_controlPoints;
    QList<QPointF> m_interpolatedPoints;
    
};


/*
1 Function to get alpha values
Use above function in apply transferfunction, applyTransferFunction is always called after update in graph

1 Function to interpolate values into QpointF for series in graph, ez?
OR return alpha values and interpolate in graph, makes more sence.

interpolatePoints, creates new interpolated points, 
which apply transfer function uses later

*/




} // namespace tfn

#endif // TRANSFERFUNCTION_H
