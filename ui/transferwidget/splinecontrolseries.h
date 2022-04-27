#ifndef SPLINECONTROLSERIES_H
#define SPLINECONTROLSERIES_H

#include "../../transferfunction.h"

#include <QApplication>
#include <QtCharts>

namespace tfn
{
class SplineControlSeries : public QObject
{
    Q_OBJECT
  public:
    SplineControlSeries(TransferFunction* tfn);
    void setAnchor(int index);
    void setVisible(bool visible);
    bool controlNodeReleased(QPointF point);
    bool controlNodeMoved(QPointF point);
    void updateControlNodes();
    bool isVisible();
    QLineSeries* getLineSeries(int n)
    {
        return n == 0 ? m_n0LineSeries : m_n1LineSeries;
    };
    QScatterSeries* getScatterSeries() { return m_controlNodesSeries; };

  public slots:
    void setClickedNode(const QPointF& point);

  private:
    TransferFunction* m_tfn;
    int m_index;
    QPointF m_pos;
    ControlPoint* m_cp;
    QPointF OFFSET = QPointF(190, 0.06);
    int m_currentClickedNode = -1;
    int m_previousClickedNode = -1;
    bool m_isLinked = false;

    QList<QPointF> m_controlNodes;
    QScatterSeries* m_controlNodesSeries = new QScatterSeries();
    QLineSeries* m_n0LineSeries = new QLineSeries();
    QLineSeries* m_n1LineSeries = new QLineSeries();
};

}; // namespace tfn

#endif