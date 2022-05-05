#ifndef TRANSFERFUNCTIONGRAPH_H
#define TRANSFERFUNCTIONGRAPH_H

#include "../properties/sharedproperties.h"
#include "../transferfunction.h"
#include "../transfertexture.h"
#include "hintitem.h"
#include "splinecontrolseries.h"

#include <QApplication>
#include <QGraphicsView>
#include <QOpenGLFunctions>
#include <QString>
#include <QtCharts>
#include <QtGlobal>

namespace tfn
{

namespace graph
{
constexpr static int WIDTH = 600;
constexpr static int HEIGHT = 600;
} // namespace graph

class TransferFunctionGraph : public QChartView
{
    Q_OBJECT
  public:
    TransferFunctionGraph(const std::shared_ptr<ISharedProperties> properties);
    void updateGraph();
    void setDisplayedColorMap(ColorMap cmap);
    void setHistogramData(std::vector<float> normalizedHistogramData);
    void setHistogramSlider(QSlider* slider) {m_histogramSlider = slider; };
  public slots:
    void updateOrRemoveClickedIndex(const QPointF& point);
    void addNewControlPoint(const QPointF& point);
    void reset();
    void setVisibleHistogram(bool checked);
    void setHistogramScaling(int value);
  protected:
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
  signals:
    void transferFunctionChanged(TransferFunction tfn);

  private:
    const std::shared_ptr<ISharedProperties> m_properties;

    // Maps local coords to chart coords
    QPointF mapLocalToChartPos(QPointF localPos);
    void updateGradient();
    void updatePlotSeries();
    void removeControlPoint(const QPointF& point);
    void updateControlPointHint(int index);
    void constructBoundingBox();
    void constructControlPointSeries();
    void constructHistogramSeries();
    void setupAxis();
    void createHistogramSeries();

    int m_currentClickedIndex = -1;
    int m_previousClickedIndex = -1;

    // Styling options
    constexpr static int POINT_SIZE = 15;
    constexpr static int LINE_WIDTH = 3;
    constexpr static QPointF HINT_OFFSET = QPointF(5, -(POINT_SIZE * 2));
    constexpr static QColor POINT_BORDER_COLOR = QColor(0, 0, 0);
    constexpr static QColor POINT_COLOR = QColor(150, 150, 150, 80);
    constexpr static QColor LINE_COLOR = QColor(165, 165, 164);

    TransferFunction m_tfn = TransferFunction();

    QChart* m_chart = new QChart();
    QLineSeries* m_lineSeries = new QLineSeries();
    QAreaSeries* m_areaSeries = new QAreaSeries();
    QScatterSeries* m_scatterSeries = new QScatterSeries();
    QAreaSeries* m_boundingBox;
    QPen* m_pen = new QPen(LINE_COLOR);
    QLinearGradient m_gradient;
    HintItem* m_hint;
    SplineControlSeries* m_splineControls;

    QLineSeries* m_histogramSeries = new QLineSeries();
    QAreaSeries* m_histogramArea = new QAreaSeries();
    std::vector<float> m_normalizedHistogramData;
    std::vector<float> m_binnedData;
    QSlider* m_histogramSlider;
    float m_histogramthreshold = 10.0f;
    bool m_isDataLoaded = false;
};

} // namespace tfn

#endif // TRANSFERFUNCTIONGRAPH_H
