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

    // Linear graident for areaSeries
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    m_lineSeries = new QLineSeries();

    // Scatterplot for controll point
    m_scatterSeries = new QScatterSeries();
    m_scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_scatterSeries->setMarkerSize(POINT_SIZE);
    m_scatterSeries->setBorderColor(POINT_BORDER_COLOR);
    m_scatterSeries->setColor(POINT_COLOR);

    // Area below line
    m_areaSeries = new QAreaSeries();
    m_areaSeries->setName("Gradient Display");
    m_areaSeries->setPen(*m_pen);

    // Bounding box for hit detection
    QLineSeries* lines = new QLineSeries();
    lines->append(QPointF(0, 0));
    lines->append(QPointF(0, 1));
    lines->append(QPointF(255, 1));
    lines->append(QPointF(255, 0));
    m_boundingBox = new QAreaSeries(lines);
    m_boundingBox->setColor(QColor(0, 0, 0, 0));

    m_chart->addSeries(m_boundingBox);
    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_scatterSeries);
    m_chart->setTitle("TransferFunction");

    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setRubberBand(QChartView::NoRubberBand);
    this->setFixedSize(300, 300); // TODO Temporary Sizing for now

    connect(m_scatterSeries, &QScatterSeries::pressed, this,
            &TransferFunctionGraph::pointPressed);

    connect(m_boundingBox, &QAreaSeries::pressed, this,
            &TransferFunctionGraph::graphClicked);

    connect(this, &TransferFunctionGraph::transferFunctionChanged,
            &m_properties.get()->transferFunction(),
            &TransferProperties::updateTransferFunction);
};

void TransferFunctionGraph::updateGraph()
{
    // TODO only update Sereies instead of remove/replace
    // Remove series, and replace with new one from control points
    m_chart->removeSeries(m_areaSeries);
    m_chart->removeSeries(m_scatterSeries);
    m_chart->removeSeries(m_boundingBox);

    m_lineSeries->clear();
    // TODO Hack to get graph to not auto scale to max values in m_lineSeries and m_scatterSeries
    m_lineSeries->append(QPointF(0, 0));
    m_lineSeries->append(QPointF(0, 1));
    m_lineSeries->append(m_tfn.getControlPoints());

    m_scatterSeries->clear();
    // TODO Hack to get graph to not auto scale to max values in m_lineSeries and m_scatterSeries
    m_scatterSeries->append(QPointF(0, 0));
    m_scatterSeries->append(QPointF(0, 1));
    m_scatterSeries->append(m_tfn.getControlPoints());

    m_areaSeries->setUpperSeries(m_lineSeries);

    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_boundingBox);
    m_chart->addSeries(m_scatterSeries);

    /* Useless to update graident each time,
    but will be usefull when you can change colors later*/
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    for (int i = 0; i < size::NumPoints; i++)
    {
        float r = m_cmap.colorMapData()[i * size::NumChannels];
        float g = m_cmap.colorMapData()[(i * size::NumChannels) + 1];
        float b = m_cmap.colorMapData()[(i * size::NumChannels) + 2];
        float a = m_cmap.colorMapData()[(i * size::NumChannels) + 3];

        QColor col;
        col.setRgbF(r, g, b);
        m_gradient.setColorAt(a, col);
    }

    m_areaSeries->setBrush(m_gradient);
    this->update();
    emit transferFunctionChanged(m_tfn);
};

void TransferFunctionGraph::setDisplayedColorMap(ColorMap cmap)
{
    m_cmap = cmap;
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    m_tfn.reset();
    updateGraph();
};

QPointF TransferFunctionGraph::mapLocalToChartPos(QPointF localpos)
{
    auto const scenePos = mapToScene(
        QPoint(static_cast<int>(localpos.x()), static_cast<int>(localpos.y())));
    auto const chartItemPos = chart()->mapFromScene(scenePos);
    auto const valueGivenSeries = chart()->mapToValue(chartItemPos);
    return valueGivenSeries;
};

QPointF TransferFunctionGraph::outOfBoundsCheck(QPointF point)
{
    QPointF max = QPointF(255, 1);
    QPointF min = QPointF(0, 0);
    return QPointF(qMax(qMin(max.x(), point.x()), min.x()),
                   qMax(qMin(max.y(), point.y()), min.y()));
};

// Sets the current clicked index of the pressed point
void TransferFunctionGraph::pointPressed(const QPointF& point)
{
    m_currentClickedIndex = m_tfn.indexOf(point);
};

// If a point is no clicked, a new point is created
void TransferFunctionGraph::graphClicked(const QPointF& point)
{
    if (m_tfn.addControlPoint(point))
    {
        updateGraph();
    }
};

// When draggins a point is finished replace point with new location
void TransferFunctionGraph::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);

    QPointF graphPoint = mapLocalToChartPos(event->localPos());
    if (m_currentClickedIndex != -1)
    {
        if (m_currentClickedIndex == 0 ||
            m_currentClickedIndex == m_tfn.getControlPoints().size() - 1)
        {
            graphPoint =
                QPointF(m_tfn.getControlPoints()[m_currentClickedIndex].x(),
                        graphPoint.y());
        }
        m_currentClickedIndex =
            m_tfn.replace(m_currentClickedIndex, outOfBoundsCheck(graphPoint));
        updateGraph();
        m_currentClickedIndex = -1;
    }
};

// Continusly replace point for firendly interactions 
void TransferFunctionGraph::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF graphPoint = mapLocalToChartPos(event->localPos());
    if (m_currentClickedIndex != -1)
    {

        if (m_currentClickedIndex == 0 ||
            m_currentClickedIndex == m_tfn.getControlPoints().size() - 1) // First point or last point click only able to move in y-direction.
        {
            graphPoint =
                QPointF(m_tfn.getControlPoints()[m_currentClickedIndex].x(),
                        graphPoint.y());
        }
        m_currentClickedIndex =
            m_tfn.replace(m_currentClickedIndex, outOfBoundsCheck(graphPoint));  // Updates currentIndex incase of reordering of controlpoints

        updateGraph();
    }
};

} // namespace tfn
