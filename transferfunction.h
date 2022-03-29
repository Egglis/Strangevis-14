#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include <QList>
#include <QPointF>
#include <QString>
#include <QOpenGLFunctions>

namespace tfn
{

class TransferFunction {
    public:
        TransferFunction();
        bool addControlPoint(QPointF pos);
        std::vector<GLfloat> applyTransferFunction(std::vector<GLfloat> cmap);
        QList<QPointF> getControlPoints() {return m_controlPoints;};
        int indexOf(QPointF point) {return m_controlPoints.indexOf(point);};
        void replace(int index, QPointF point) {m_controlPoints.replace(index, point);};
        void clear() {m_controlPoints.clear();};
        void reset();
    private:
        float solveLineEquation(QPointF p0, QPointF p1, int t);

        QString m_type;
        QList<QPointF> m_controlPoints;
};

}

#endif // TRANSFERFUNCTION_H