#ifndef TRANSFERFUNCTIONGRAPH_H
#define TRANSFERFUNCTIONGRAPH_H

#include "../properties/sharedproperties.h"
#include "../transferfunction.h"
#include "../transfertexture.h"

#include <QGraphicsView>
#include <QtCharts>
#include <QtGlobal>

// TODO add removing of conrol points

namespace tfn
{
class TransferFunctionGraph : public QChartView
{
    Q_OBJECT
  public:
    TransferFunctionGraph(const std::shared_ptr<ISharedProperties> properties);
    void updateGraph();

    void setDisplayedColorMap(ColorMap cmap);
  public slots:
    void updateClickedIndex(const QPointF& point);
    void addNewControlPoint(const QPointF& point);

  protected:
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
  signals:
    void transferFunctionChanged(TransferFunction tfn);

  private:
    const std::shared_ptr<ISharedProperties> m_properties;

    // Maps local coords to chart coords
    QPointF mapLocalToChartPos(QPointF localPos);
    // Checks if point is out of bounds and returns max x/y pointF
    QPointF clampToDomain(QPointF point);
    void updateGradient();
    void updatePlotSeries();

    int m_currentClickedIndex = -1;

    // Styling options
    constexpr static int POINT_SIZE = 15;
    constexpr static int LINE_WIDTH = 3;
    constexpr static QColor POINT_BORDER_COLOR = QColor(0, 0, 0);
    constexpr static QColor POINT_COLOR = QColor(150, 150, 150, 0.4);
    constexpr static QColor LINE_COLOR = QColor(165, 165, 164);

    TransferFunction m_tfn = TransferFunction();
    ColorMap m_cmap;

    QChart* m_chart = new QChart();
    QLineSeries* m_lineSeries = new QLineSeries();
    QAreaSeries* m_areaSeries = new QAreaSeries();
    QScatterSeries* m_scatterSeries = new QScatterSeries();
    QAreaSeries* m_boundingBox;
    QPen* m_pen = new QPen(LINE_COLOR);
    QLinearGradient m_gradient;
};
} // namespace tfn

#endif