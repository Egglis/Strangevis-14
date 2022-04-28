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

enum class Nodes
{
    NODE0,
    NODE1,
    INVALID_NODE 

};

class ColorMap;

class ControlPoint : public QPointF
{
  public:
    ControlPoint(QPointF position);
    void setControlNode(Nodes node, QPointF pos);
    void setAllControlNodes(QList<QPointF> nodes);
    QMap<Nodes, QPointF> getControlNodes() { return m_controlNodes; };

  private:
     QMap<Nodes, QPointF> m_controlNodes;
};

class TransferFunction
{
  public:
    TransferFunction();
    void reset();
    bool addControlPoint(ControlPoint cp);
    bool removeControlPoint(ControlPoint point);
    void setColorMap(ColorMap cmap);
    void updateTransferFunction();
    void applyTransferFunction();
    const std::vector<GLfloat>& getColorMapData() const { return m_cmapData; };
    void interpolatePoints();

    QList<ControlPoint> getControlPoints() { return m_controlPoints; };
    int indexOf(ControlPoint point) { return m_controlPoints.indexOf(point); };
    void replace(int index, ControlPoint point);

    void setControlNodePos(int index, Nodes node, QPointF pos);

    QList<QPointF> getInterpolatedPoints() { return m_interpolatedPoints; };

  private:
    constexpr static QPointF deCasteljau(QPointF p0, QPointF p1, QPointF p2,
                                         QPointF p3, float t);
    constexpr static float getPt(float n1, float n2, float perc);
    QPointF clampToNeighbours(int index, ControlPoint point);
    QPointF clampToDomain(ControlPoint point, QPointF min, QPointF max);

    QList<ControlPoint> m_controlPoints;
    QList<QPointF> m_interpolatedPoints;
    std::vector<GLfloat> m_cmapData;
};

} // namespace tfn

#endif // TRANSFERFUNCTION_H
