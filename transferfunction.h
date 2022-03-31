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
constexpr static QPointF END_POINT = QPointF(255,1);
}; // namespace points

class TransferFunction
{
  public:
    TransferFunction();
    bool addControlPoint(QPointF pos);
    std::vector<GLfloat> applyTransferFunction(const std::vector<GLfloat> cmap);
    QList<QPointF> getControlPoints() { return m_controlPoints; };
    int indexOf(QPointF point) { return m_controlPoints.indexOf(point); };
    int replace(int index, QPointF point);
    void clear() { m_controlPoints.clear(); };
    void reset();

  private:
    // Uses the line equation y = mx + b to interpolate alpha values between points
    constexpr static float getInterpolatedValueBetweenPoints(QPointF p0, QPointF p1, int t);

    QList<QPointF> m_controlPoints;
};

} // namespace tfn

#endif // TRANSFERFUNCTION_H