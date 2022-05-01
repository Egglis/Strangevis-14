#include "transferfunctiongraph.h"

namespace tfn
{

TransferFunctionGraph::TransferFunctionGraph(
    const std::shared_ptr<ISharedProperties> properties)
    : m_properties{properties}
{
    m_chart->legend()->hide();
    m_chart->setBackgroundVisible(false);
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    m_chart->setBackgroundRoundness(0);
    m_chart->setContentsMargins(-30,-30,-30,-30);
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    constructControlPointSeries();

    m_hint = new HintItem(m_chart, m_scatterSeries);
    m_hint->setZValue(11);
    m_hint->hide();

    m_splineControls = new SplineControlSeries(&m_tfn);
    m_splineControls->setVisible(false);

    constructBoundingBox();

    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_boundingBox);
    m_chart->addSeries(m_splineControls->getLineSeries(Nodes::NODE0));
    m_chart->addSeries(m_splineControls->getLineSeries(Nodes::NODE1));
    m_chart->addSeries(m_scatterSeries);
    m_chart->addSeries(m_splineControls->getScatterSeries());


    setupAxis();

    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setRubberBand(QChartView::NoRubberBand);
    this->setStyleSheet("background: transparent");
    connect(m_scatterSeries, &QScatterSeries::pressed, this,
            &TransferFunctionGraph::updateOrRemoveClickedIndex);

    connect(m_boundingBox, &QAreaSeries::pressed, this,
            &TransferFunctionGraph::addNewControlPoint);

    connect(this, &TransferFunctionGraph::transferFunctionChanged,
            &m_properties.get()->transferFunction(),
            &TransferProperties::updateTransferFunction);
};

void TransferFunctionGraph::reset(){
    m_tfn.reset();
    updateGraph();
    m_splineControls->setVisible(false);
}

void TransferFunctionGraph::constructBoundingBox()
{
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
}

void TransferFunctionGraph::constructControlPointSeries()
{
    m_scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_scatterSeries->setMarkerSize(POINT_SIZE);
    m_scatterSeries->setBorderColor(POINT_BORDER_COLOR);
    m_scatterSeries->setColor(POINT_COLOR);

    m_pen->setWidth(LINE_WIDTH);
    m_areaSeries->setName("Gradient Display");
    m_areaSeries->setPen(*m_pen);
    m_areaSeries->setUpperSeries(m_lineSeries);
}

void TransferFunctionGraph::updateGraph()
{
    m_tfn.updateTransferFunction();
    updatePlotSeries();
    updateGradient();
    this->update();
    emit transferFunctionChanged(m_tfn);
};

void TransferFunctionGraph::setupAxis()
{
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
}

void TransferFunctionGraph::updatePlotSeries()
{
    QList<QPointF> scatterSeries;
    for (ControlPoint cp : m_tfn.getControlPoints())
    {
        scatterSeries.append(static_cast<QPointF>(cp));
    }
    m_lineSeries->replace(m_tfn.getInterpolatedPoints());
    m_scatterSeries->replace(scatterSeries);
};

void TransferFunctionGraph::updateGradient()
{
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    auto cmap = m_tfn.getColorMapData();
    for (int i = 0; i < tfn::size::NUM_POINTS; i++)
    {
        float r = cmap.at(i * tfn::size::NUM_CHANNELS);
        float g = cmap.at(i * tfn::size::NUM_CHANNELS + 1);
        float b = cmap.at(i * tfn::size::NUM_CHANNELS + 2);
        float a = cmap.at(i * tfn::size::NUM_CHANNELS + 3);

        QColor col;
        col.setRgbF(r, g, b, qMin(a, 1.0f));
        m_gradient.setColorAt(i / static_cast<float>(tfn::size::NUM_POINTS),
                              col);
    }
    m_areaSeries->setBrush(m_gradient);
};

void TransferFunctionGraph::setDisplayedColorMap(ColorMap cmap)
{
    m_tfn.setColorMap(cmap);
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
        const Qt::MouseButtons pressedButton = QGuiApplication::mouseButtons();
        if (pressedButton == Qt::LeftButton)
        {
            m_currentClickedIndex = m_tfn.indexOf(point);
            m_splineControls->setAnchor(m_currentClickedIndex);
            m_splineControls->setVisible(
                m_currentClickedIndex == m_previousClickedIndex &&
                m_currentClickedIndex != m_tfn.getControlPoints().length()-1);
        }
        else if (pressedButton == Qt::RightButton)
        {
            removeControlPoint(point);
            m_splineControls->setVisible(false);
            m_currentClickedIndex = -1;
        }
    }
};

// If an empty spot on the graph is clicked, a new point is created
void TransferFunctionGraph::addNewControlPoint(const QPointF& point)
{
    const Qt::MouseButtons button = QGuiApplication::mouseButtons();
    if (button == Qt::LeftButton && m_tfn.addControlPoint(point))
    {
        m_splineControls->setVisible(false);
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

    if (m_splineControls->controlNodeReleased(graphPoint))
    {
        updateGraph();
    };
    if (m_currentClickedIndex != -1)
    {
        m_tfn.replace(m_currentClickedIndex, graphPoint);
        m_splineControls->setAnchor(m_currentClickedIndex);
        m_previousClickedIndex = m_currentClickedIndex;
        m_currentClickedIndex = -1;
        m_hint->hide();
        updateGraph();
    }
};

// Mouse event for handeling dragging of a point
void TransferFunctionGraph::mouseMoveEvent(QMouseEvent* event)
{
    QChartView::mouseMoveEvent(event);
    QPointF graphPoint = mapLocalToChartPos(event->position());
    if (m_splineControls->controlNodeMoved(graphPoint))
    {
        updateGraph();
    }
    if (m_currentClickedIndex != -1)
    {
        m_tfn.replace(m_currentClickedIndex, graphPoint);
        m_splineControls->setAnchor(m_currentClickedIndex);
        updateControlPointHint(m_currentClickedIndex);
        m_hint->show();
        updateGraph();
    }
};

void TransferFunctionGraph::updateControlPointHint(int index)
{
    QPointF point = m_tfn.getControlPoints().at(index);
    double x = point.x();
    double y = point.y();
    m_hint->setText(QString("X: %1 Y: %2")
                        .arg(QString::number(x, 'f', 2))
                        .arg(QString::number(y, 'f', 2)));
    m_hint->setAnchor(point, HINT_OFFSET);
};

} // namespace tfn
