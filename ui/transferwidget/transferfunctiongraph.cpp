#include "transferfunctiongraph.h"

namespace tfn
{

TransferFunctionGraph::TransferFunctionGraph(
    const std::shared_ptr<ISharedProperties> properties)
    : m_properties{properties}
{

    m_chart->legend()->hide();
    m_pen->setWidth(LINE_WIDTH);

    // Linear gradient for areaSeries
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    // Used to display control points as scatterplot
    m_scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_scatterSeries->setMarkerSize(POINT_SIZE);
    m_scatterSeries->setBorderColor(POINT_BORDER_COLOR);
    m_scatterSeries->setColor(POINT_COLOR);

    // Used to display gradient below the line
    m_areaSeries->setName("Gradient Display");
    m_areaSeries->setPen(*m_pen);

    // Control Point hint
    m_hint = new HintItem(m_chart, m_scatterSeries);
    m_hint->setZValue(11);
    m_hint->hide();



    // Bounding box for hit detection
    QLineSeries* lines = new QLineSeries();
    lines->append(
        QPointF(tfn::points::START_POINT.x(), tfn::points::START_POINT.y()));
    lines->append(
        QPointF(tfn::points::START_POINT.x(), tfn::points::END_POINT.y()));
    lines->append(
        QPointF(tfn::points::END_POINT.x(), tfn::points::END_POINT.y()));
    lines->append(
        QPointF(tfn::points::END_POINT.x(), tfn::points::START_POINT.y()));
    m_boundingBox = new QAreaSeries(lines);
    m_boundingBox->setColor(QColor(0, 0, 0, 0));
    m_areaSeries->setUpperSeries(m_lineSeries);

    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_boundingBox);
    m_splineControls = new SplineControlSeries(&m_tfn, m_chart);
    m_splineControls->setVisible(true);
    m_chart->addSeries(m_scatterSeries);
    m_chart->setBackgroundVisible(false);
    m_chart->createDefaultAxes();
    auto axes = m_chart->axes();
    axes[0]->setRange(tfn::points::START_POINT.x(), tfn::points::END_POINT.x());
    axes[1]->setRange(tfn::points::START_POINT.y(), tfn::points::END_POINT.y());
    axes[0]->setLabelsVisible(false);
    axes[1]->setLabelsVisible(false);

    m_areaSeries->attachAxis(axes[0]);
    m_areaSeries->attachAxis(axes[1]);
    m_scatterSeries->attachAxis(axes[0]);
    m_scatterSeries->attachAxis(axes[1]);

    m_chart->setTitle("TransferFunction");

    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setRubberBand(QChartView::NoRubberBand);
    this->setStyleSheet("background: transparent");
    this->setFixedSize(300, 300); // TODO Temporary Sizing for now
    connect(m_scatterSeries, &QScatterSeries::pressed, this,
            &TransferFunctionGraph::updateOrRemoveClickedIndex);

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

void TransferFunctionGraph::updatePlotSeries()
{
    m_lineSeries->replace(m_tfn.m_b);
    m_scatterSeries->replace(m_tfn.getSeriesPoints());
};

void TransferFunctionGraph::updateGradient()
{
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    std::vector<GLfloat> cmap =
        m_tfn.applyTransferFunction(m_cmap.colorMapData());

    for (int i = 0; i < tfn::size::NUM_POINTS; i++)
    {
        float r = cmap[i * tfn::size::NUM_CHANNELS];
        float g = cmap[(i * tfn::size::NUM_CHANNELS) + 1];
        float b = cmap[(i * tfn::size::NUM_CHANNELS) + 2];
        float a = cmap[(i * tfn::size::NUM_CHANNELS) + 3];

        QColor col;
        col.setRgbF(r, g, b, qMin(a, static_cast<float>(1)));
        m_gradient.setColorAt(i / static_cast<float>(tfn::size::NUM_POINTS),
                              col);
    }
    m_areaSeries->setBrush(m_gradient);
};

void TransferFunctionGraph::setDisplayedColorMap(ColorMap cmap)
{
    m_cmap = cmap;
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

// Either sets current clicked index or removes the point
void TransferFunctionGraph::updateOrRemoveClickedIndex(const QPointF& point)
{
    if (m_currentClickedIndex == -1)
    {
        Qt::MouseButtons pressedButton = QGuiApplication::mouseButtons();
        if (pressedButton == Qt::LeftButton)
        {
            m_currentClickedIndex = m_tfn.indexOf(point);
            updateControlPointHint(m_currentClickedIndex);
            m_splineControls->setVisible(false);

            if (m_currentClickedIndex == m_previousClickedIndex)
            {
                qDebug() << "Double Clicked Point";
                m_splineControls->setAnchor(m_currentClickedIndex);
                m_splineControls->setVisible(true);
            }
        }
        else if (pressedButton == Qt::RightButton)
        {
            removeControlPoint(point);
        }
    }
};

// If an empty spot on the graph is clicked, a new point is created
void TransferFunctionGraph::addNewControlPoint(const QPointF& point)
{
    Qt::MouseButtons button = QGuiApplication::mouseButtons();
    if (button == Qt::LeftButton && m_tfn.addControlPoint(point))
    {
        updateGraph();
    }
};

void TransferFunctionGraph::removeControlPoint(const QPointF& point)
{
    if (m_tfn.removeControlPoint(point))
    {
        updateGraph();
    }
};

// When draggins a point is finished replace point with new location
void TransferFunctionGraph::mouseReleaseEvent(QMouseEvent* event)
{
    QChartView::mouseReleaseEvent(event);
    QPointF graphPoint = mapLocalToChartPos(event->position());

    m_splineControls->controlNodeReleased(graphPoint);
    if (m_currentClickedIndex != -1)
    {
        m_tfn.replace(m_currentClickedIndex, graphPoint);
        m_previousClickedIndex = m_currentClickedIndex;
        m_currentClickedIndex = -1;
        m_hint->hide();

    }
    updateGraph();

};

// Mouse event for handeling dragging of a point
void TransferFunctionGraph::mouseMoveEvent(QMouseEvent* event)
{
    QChartView::mouseMoveEvent(event);
    QPointF graphPoint = mapLocalToChartPos(event->position());
    m_splineControls->controlNodeMoved(graphPoint);
    if (m_currentClickedIndex != -1)
    {
        m_tfn.replace(m_currentClickedIndex, graphPoint);
        updateControlPointHint(m_currentClickedIndex);
        m_splineControls->updateControlNodes();
        m_hint->show();
    }
            updateGraph();

};

void TransferFunctionGraph::updateControlPointHint(int index)
{
    QPointF point = m_tfn.getControlPoints().at(index);
    m_hint->setText(QString("X: %1 Y: %2").arg(point.x()).arg(point.y()));
    m_hint->setAnchor(point, HINT_OFFSET);
};

} // namespace tfn
