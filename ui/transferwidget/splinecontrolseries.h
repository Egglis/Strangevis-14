#ifndef SPLINECONTROLSERIES_H
#define SPLINECONTROLSERIES_H

#include "../../transferfunction.h"

#include <QtCharts>
#include <QApplication>


namespace tfn
{
class SplineControlSeries : public QObject
{
    Q_OBJECT
  public:
    SplineControlSeries(TransferFunction* tfn, QChart* chart);
    void setAnchor(QPointF pos, int index);
    void setVisible(bool visible);
    void controlNodeReleased(QPointF point);
    void controlNodeMoved(QPointF point);
  public slots:
    void setClickedIndex(const QPointF& point);
  private:
    void updateControlNodes();
    TransferFunction* m_tfn;
    QChart* m_chart;
    int m_index;
    QPointF m_pos;
    QPointF OFFSET = QPointF(190, 0.06);
    int m_currentClickedIndex = -1;

    QList<QPointF> m_controlNodes;
    QScatterSeries* m_controlNodesSeries = new QScatterSeries();
    QLineSeries* m_top = new QLineSeries();
    QLineSeries* m_right = new QLineSeries();
    QLineSeries* m_bottom = new QLineSeries();
    QLineSeries* m_left = new QLineSeries();
};

}; // namespace tfn

#endif