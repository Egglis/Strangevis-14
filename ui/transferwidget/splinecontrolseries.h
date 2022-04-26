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
    void setAnchor(int index);
    void setVisible(bool visible);
    void controlNodeReleased(QPointF point);
    void controlNodeMoved(QPointF point);
    void updateControlNodes();

  public slots:
    void setClickedIndex(const QPointF& point);
  signals:
    void controlNodesChanged();
  private:
    TransferFunction* m_tfn;
    QChart* m_chart;
    int m_index;
    QPointF m_pos;
    ControlPoint* m_cp;
    QPointF OFFSET = QPointF(190, 0.06);
    int m_currentClickedNode = -1;

    QList<QPointF> m_controlNodes;
    QScatterSeries* m_controlNodesSeries = new QScatterSeries();
    QLineSeries* m_top = new QLineSeries();
    QLineSeries* m_right = new QLineSeries();
};

}; // namespace tfn

#endif