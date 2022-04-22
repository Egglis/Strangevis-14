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

class TransferFunction
{
  public:
    TransferFunction();
    bool addControlPoint(QPointF pos);
    bool removeControlPoint(QPointF point);
    std::vector<GLfloat> applyTransferFunction(const std::vector<GLfloat> cmap);
    QList<QPointF> getControlPoints() { return m_controlPoints; };
    int indexOf(QPointF point) { return m_controlPoints.indexOf(point); };
    void replace(int index, QPointF point);
    void reset();

  private:
    // Uses the line equation y = mx + b to interpolate alpha values between
    // points
    constexpr static float getInterpolatedValueBetweenPoints(QPointF p0,
                                                             QPointF p1, int t);
    QPointF clampToDomain(int index, QPointF point);

    QList<QPointF> m_controlPoints;
};

} // namespace tfn

#endif // TRANSFERFUNCTION_H
