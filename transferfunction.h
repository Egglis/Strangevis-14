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

class ControlPoint : public QPointF
{
  public:
    ControlPoint(QPointF position);
    void setControlNode(int index, QPointF pos);
  private:
    QList<QPointF> m_controlNodes;   
};

class TransferFunction
{
  public:
    TransferFunction();
    bool addControlPoint(ControlPoint cp);
    bool removeControlPoint(ControlPoint point);
    std::vector<GLfloat> applyTransferFunction(const std::vector<GLfloat> cmap);
    QList<QPointF> getSeriesPoints();
    QList<ControlPoint> getControlPoints() { return m_controlPoints; };
    int indexOf(ControlPoint point) { return m_controlPoints.indexOf(point); };
    void replace(int index, ControlPoint point);
    void reset();

  private:
    // Uses the line equation y = mx + b to interpolate alpha values between
    // points
    constexpr static float getInterpolatedValueBetweenPoints(QPointF p0,
                                                             QPointF p1, int t);
    QPointF clampToDomain(int index, ControlPoint cp);

    QList<ControlPoint> m_controlPoints;
};





} // namespace tfn

#endif // TRANSFERFUNCTION_H
