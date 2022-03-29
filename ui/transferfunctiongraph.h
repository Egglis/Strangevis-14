#ifndef TRANSFERFUNCTIONGRAPH_H
#define TRANSFERFUNCTIONGRAPH_H

#include "../transfertexture.h"
#include "../transferfunction.h"
#include "../properties/sharedproperties.h"


#include <QGraphicsView>
#include <QtCharts>

namespace tfn {
class TransferFunctionGraph : public QChartView
{
        Q_OBJECT
    public:
        TransferFunctionGraph(const std::shared_ptr<ISharedProperties> properties);
        void updateGraph();
        void setDisplayedColorMap(ColorMap cmap);
    public slots:
        void pointPressed(const QPointF &point);
        void graphClicked(const QPointF &point);
    protected:
        void mouseReleaseEvent(QMouseEvent* event);
        void mouseMoveEvent(QMouseEvent* event);
    signals:
        void transferFunctionAltered(ColorMap cmap);
        void transferFunctionChanged(TransferFunction tfn);
    private:
        const std::shared_ptr<ISharedProperties> m_properties;

        // Maps local coords to chart coords
        QPointF mapLocalToChartPos(QPointF localPos);

        // Solve line equation between p0, p1 at t
        float solveSlopeEquation(QPointF p0, QPointF p1, int t);

        void applyColorMapChanges();

        int m_currentClickedIndex = -1;

        int m_test = 0;

        // Styling options
        int POINT_SIZE = 15;
        int LINE_WIDTH = 3;
        QColor POINT_BORDER_COLOR = QColor(0,0,0);
        QColor POINT_COLOR = QColor(150,150,150,0.4);
        QColor LINE_COLOR = QColor(165, 165, 164);

        // Qwidgets
        ColorMap m_cmap;
        QChart *m_chart;
        TransferFunction m_tfn;
        QLineSeries *m_lineSeries;
        QAreaSeries *m_areaSeries;
        QAreaSeries *m_boundingBox;
        QScatterSeries *m_scatterSeries;
        QPen *m_pen;
        QLinearGradient m_gradient;

};
}

#endif