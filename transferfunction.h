#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include <QList>
#include <QOpenGLFunctions>
#include <QPointF>
#include <QString>

namespace tfn
{

class TransferFunction
{
  public:
    TransferFunction();
    bool addControlPoint(QPointF pos);
    std::vector<GLfloat> applyTransferFunction(std::vector<GLfloat> cmap);
    QList<QPointF> getControlPoints() { return m_controlPoints; };
    int indexOf(QPointF point) { return m_controlPoints.indexOf(point); };
    int replace(int index, QPointF point);
    void clear() { m_controlPoints.clear(); };
    void reset();

  private:
    // Solve the line equation y = mx + b, where t = x;
    float solveLineEquation(QPointF p0, QPointF p1, int t);

    // m_types is usefull later when implementing spline or curved functions
    QString m_type;
    QList<QPointF> m_controlPoints;
};

} // namespace tfn

#endif // TRANSFERFUNCTION_H