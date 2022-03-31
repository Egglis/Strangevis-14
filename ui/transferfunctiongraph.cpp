#include "transferfunctiongraph.h"

namespace tfn
{

TransferFunctionGraph::TransferFunctionGraph(
    const std::shared_ptr<ISharedProperties> properties)
    : m_properties{properties}
{
    m_tfn = TransferFunction();

    m_chart = new QChart();
    m_chart->legend()->hide();

    m_pen = new QPen(LINE_COLOR);
    m_pen->setWidth(LINE_WIDTH);

    // Linear gradient for areaSeries
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    m_lineSeries = new QLineSeries();

    // Used to display control points as scatterplot
    m_scatterSeries = new QScatterSeries();
    m_scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_scatterSeries->setMarkerSize(POINT_SIZE);
    m_scatterSeries->setBorderColor(POINT_BORDER_COLOR);
    m_scatterSeries->setColor(POINT_COLOR);

    // Used to display gradient below the line
    m_areaSeries = new QAreaSeries();
    m_areaSeries->setName("Gradient Display");
    m_areaSeries->setPen(*m_pen);

    // Bounding box for hit detection
    QLineSeries* lines = new QLineSeries();
    lines->append(QPointF(tfn::points::START_POINT.x(), tfn::points::START_POINT.y()));
    lines->append(QPointF(tfn::points::START_POINT.x(), tfn::points::END_POINT.y()));
    lines->append(QPointF(tfn::points::END_POINT.x(), tfn::points::END_POINT.y()));
    lines->append(QPointF(tfn::points::END_POINT.x(), tfn::points::START_POINT.y()));
    m_boundingBox = new QAreaSeries(lines);
    m_boundingBox->setColor(QColor(0, 0, 0, 0));

    m_chart->addSeries(m_boundingBox);
    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_scatterSeries);
    m_chart->createDefaultAxes();
    auto axes = m_chart->axes();
    axes[0]->setRange(tfn::points::START_POINT.x(), tfn::points::END_POINT.x());
    axes[1]->setRange(tfn::points::START_POINT.y(), tfn::points::END_POINT.y());

    m_chart->setTitle("TransferFunction");

    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setRubberBand(QChartView::NoRubberBand);
    this->setFixedSize(300, 300); // TODO Temporary Sizing for now

    connect(m_scatterSeries, &QScatterSeries::pressed, this,
            &TransferFunctionGraph::updateClickedIndex);

    connect(m_boundingBox, &QAreaSeries::pressed, this,
            &TransferFunctionGraph::addNewControlPoint);

    connect(this, &TransferFunctionGraph::transferFunctionChanged,
            &m_properties.get()->transferFunction(),
            &TransferProperties::updateTransferFunction);
};

void TransferFunctionGraph::updateGraph()
{
    updatePlotSeries();
    updateGradient();
    this->update();
    emit transferFunctionChanged(m_tfn);
};

void TransferFunctionGraph::updatePlotSeries(){
    m_chart->removeSeries(m_areaSeries);
    m_chart->removeSeries(m_scatterSeries);
    m_chart->removeSeries(m_boundingBox);

    m_lineSeries->clear();
    m_lineSeries->append(m_tfn.getControlPoints());

    m_scatterSeries->clear();
    m_scatterSeries->append(m_tfn.getControlPoints());

    m_areaSeries->setUpperSeries(m_lineSeries);

    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_boundingBox);
    m_chart->addSeries(m_scatterSeries);

    m_scatterSeries->attachAxis(m_chart->axisX());
    m_scatterSeries->attachAxis(m_chart->axisY());
    m_lineSeries->attachAxis(m_chart->axisX());
    m_lineSeries->attachAxis(m_chart->axisY());
    m_areaSeries->attachAxis(m_chart->axisX());
    m_areaSeries->attachAxis(m_chart->axisY());
};

void TransferFunctionGraph::updateGradient(){
    /* Useless to update graident each time,
    but will be usefull when you can change colors later */
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    for (int i = 0; i < tfn::size::NUM_POINTS; i++)
    {
        float r = m_cmap.colorMapData()[i * tfn::size::NUM_CHANNELS];
        float g = m_cmap.colorMapData()[(i * tfn::size::NUM_CHANNELS) + 1];
        float b = m_cmap.colorMapData()[(i * tfn::size::NUM_CHANNELS) + 2];
        float a = m_cmap.colorMapData()[(i * tfn::size::NUM_CHANNELS) + 3];

        QColor col;
        col.setRgbF(r, g, b);
        m_gradient.setColorAt(a, col);
    }

    m_areaSeries->setBrush(m_gradient);
};

void TransferFunctionGraph::setDisplayedColorMap(ColorMap cmap)
{
    m_cmap = cmap;
    m_tfn.reset();
    updateGraph();
};

QPointF TransferFunctionGraph::mapLocalToChartPos(QPointF localpos)
{
    auto scenePos = mapToScene(
        QPoint(static_cast<int>(localpos.x()), static_cast<int>(localpos.y())));
    auto chartItemPos = chart()->mapFromScene(scenePos);
    auto chartPos = m_chart->mapToValue(chartItemPos, m_boundingBox);
    return chartPos;
};

QPointF TransferFunctionGraph::clampToDomain(QPointF point)
{
    QPointF max = tfn::points::END_POINT;
    QPointF min = tfn::points::START_POINT;
    return QPointF(qMax(qMin(max.x() - 0.001, point.x()), min.x() + 0.001),
                   qMax(qMin(max.y(), point.y()), min.y()));
};

// Sets the current clicked index of the pressed point
void TransferFunctionGraph::updateClickedIndex(const QPointF& point)
{
    m_currentClickedIndex = m_tfn.indexOf(point);
};

// If a point is no clicked, a new point is created
void TransferFunctionGraph::addNewControlPoint(const QPointF& point)
{
    if (m_tfn.addControlPoint(point))
    {
        updateGraph();
    }
};

// When draggins a point is finished replace point with new location
void TransferFunctionGraph::mouseReleaseEvent(QMouseEvent* event)
{
    QChartView::mouseReleaseEvent(event);

    QPointF graphPoint = mapLocalToChartPos(event->localPos());
    if (m_currentClickedIndex != -1)
    {
        if (m_currentClickedIndex == 0 ||
            m_currentClickedIndex == m_tfn.getControlPoints().size() - 1)
        {
            graphPoint = QPointF(m_tfn.getControlPoints()[m_currentClickedIndex].x(),qMax(qMin(tfn::points::END_POINT.y(), graphPoint.y()), tfn::points::START_POINT.y()));
            m_tfn.replace(m_currentClickedIndex, graphPoint);
        } else {
            m_tfn.replace(m_currentClickedIndex, clampToDomain(graphPoint));
        }
        updateGraph();
        m_currentClickedIndex = -1;
    }
};

// Mouse event for handeling dragging of a point
void TransferFunctionGraph::mouseMoveEvent(QMouseEvent* event)
{
    QChartView::mouseMoveEvent(event);

    QPointF graphPoint = mapLocalToChartPos(event->localPos());
    if (m_currentClickedIndex != -1)
    {
        // First point or last point click only able to move in y-direction.
        if (m_currentClickedIndex == 0 ||
            m_currentClickedIndex == m_tfn.getControlPoints().size() - 1)
        {
            graphPoint =
                QPointF(m_tfn.getControlPoints()[m_currentClickedIndex].x(),
                        qMax(qMin(tfn::points::END_POINT.y(), graphPoint.y()),
                             tfn::points::START_POINT.y()));
            
            graphPoint = QPointF(m_tfn.getControlPoints()[m_currentClickedIndex].x(), graphPoint.y());
            m_currentClickedIndex = m_tfn.replace(m_currentClickedIndex, graphPoint);
        } else {
            m_currentClickedIndex = m_tfn.replace(m_currentClickedIndex, clampToDomain(graphPoint));
        }
        updateGraph();
    }
};

} // namespace tfn
