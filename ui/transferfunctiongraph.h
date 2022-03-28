#ifndef TRANSFERFUNCTIONGRAPH_H
#define TRANSFERFUNCTIONGRAPH_H

#include "../transfertexture.h"

#include <QGraphicsView>
#include <QtCharts>

namespace tfn {
class TransferFunctionGraph : public QChartView
{
    Q_OBJECT
  public:
        TransferFunctionGraph();
        void updateGraph();
        void setDisplayedColorMap(ColorMap cmap);
  private:
        ColorMap m_cmap;
        QChart *m_chart;
        QLineSeries *m_lineSeries;
        QAreaSeries *m_areaSeries;
        QScatterSeries *m_scatterSeries;
        QPen *m_pen;
        QLinearGradient m_gradient;
};
}

#endif