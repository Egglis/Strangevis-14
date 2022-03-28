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
        void drawGraph();
        void setDisplayedColorMap(ColorMap cmap);
    public slots:
        void pointClicked(const QPointF &point);
    protected:
        virtual void mousePressEvent(QMouseEvent* p_event) override;
        virtual void mouseReleaseEvent(QMouseEvent* p_event) override;
        virtual void mouseMoveEvent(QMouseEvent* p_event) override;
    private:
        bool addControllPoint(QPointF pos);

        ColorMap m_cmap;
        QList<QPointF> m_controllPoints;

        QChart *m_chart;
        QLineSeries *m_lineSeries;
        QAreaSeries *m_areaSeries;
        QScatterSeries *m_scatterSeries;
        QPen *m_pen;
        QLinearGradient m_gradient;
};
}

#endif