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
namespace nodes
{
constexpr static int NUM_NODES = 2;
constexpr static int NODE0 = 0;
constexpr static int NODE1 = 1;
} // namespace nodes

class ControlPoint : public QPointF
{
  public:
    ControlPoint(QPointF position);
    void setControlNode(int node, QPointF pos);
    void setAllControlNodes(QList<QPointF> nodes);
    QList<QPointF> getControlNodes() { return m_controlNodes; };

  private:
    QList<QPointF> m_controlNodes;
};

class TransferFunction
{
  public:
    TransferFunction();
    void reset();
    bool addControlPoint(ControlPoint cp);
    bool removeControlPoint(ControlPoint point);
    std::vector<GLfloat> applyTransferFunction(const std::vector<GLfloat> cmap);
    void interpolatePoints();

    QList<ControlPoint> getControlPoints() { return m_controlPoints; };
    int indexOf(ControlPoint point) { return m_controlPoints.indexOf(point); };
    void replace(int index, ControlPoint point);

    QPointF deCasteljau(QPointF p0, QPointF p1, QPointF p2, QPointF p3,
                        float t);
    void setControlNodePos(int index, int node, QPointF pos);

    QList<QPointF> getInterpolatedPoints() { return m_interpolatedPoints; };

  private:
    constexpr static float getPt(float n1, float n2, float perc);
    QPointF clampToNeighbours(int index, ControlPoint point);
    QPointF clampToDomain(ControlPoint point, QPointF max, QPointF min);

    QList<ControlPoint> m_controlPoints;
    QList<QPointF> m_interpolatedPoints;
};

} // namespace tfn

#endif // TRANSFERFUNCTION_H
