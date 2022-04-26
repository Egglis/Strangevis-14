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
constexpr static QPointF END_POINT = QPointF(4095, 1);
}; // namespace points
namespace nodes {
  constexpr static QPointF OFFSET = QPointF(190, 0.06);
  constexpr static int RIGHT = 0;
  constexpr static int TOP = 1;
}

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
    QList<QPointF> toLineSeries();
    QList<QPointF> getSeriesPoints();
    QList<ControlPoint> getControlPoints() { return m_controlPoints; };
    int indexOf(ControlPoint point) { return m_controlPoints.indexOf(point); };
    void replace(int index, ControlPoint point);
    void reset();
    QPointF bezier(QPointF p0, QPointF p1, QPointF p2, QPointF p3, float t);
    void setControlNodePos(int index, int dir, QPointF pos);
    QList<QPointF> m_b;
  private:
    float getPt(float n1, float n2, float perc);
    // Uses the line equation y = mx + b to interpolate alpha values between
    // points
    constexpr static float getInterpolatedValueBetweenPoints(QPointF p0,
                                                             QPointF p1, int t);
    QPointF clampToDomain(int index, ControlPoint cp);

    QList<ControlPoint> m_controlPoints;
};





} // namespace tfn

#endif // TRANSFERFUNCTION_H
