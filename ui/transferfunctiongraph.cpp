#include "transferfunctiongraph.h"

namespace tfn {


TransferFunctionGraph::TransferFunctionGraph(const std::shared_ptr<ISharedProperties> properties) : m_properties{properties}
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

    QLineSeries* lines = new QLineSeries();
    lines->append(QPointF(0,0));
    lines->append(QPointF(0,1));
    lines->append(QPointF(255,1));
    lines->append(QPointF(255,0));
    m_boundingBox = new QAreaSeries(lines);
    m_boundingBox->setColor(QColor(0,0,0,0));
    

    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_boundingBox);
    m_chart->addSeries(m_scatterSeries);
    m_chart->createDefaultAxes();
    m_chart->setTitle("Test Graph");

    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setFixedSize(300, 300); // TODO Temporary


    connect(m_scatterSeries, &QScatterSeries::pressed,
            this, &TransferFunctionGraph::pointPressed); 

    connect(m_boundingBox, &QAreaSeries::pressed,
            this, &TransferFunctionGraph::graphClicked);

    connect(this, &TransferFunctionGraph::transferFunctionChanged,
            &m_properties.get()->transferFunction(), &TransferProperties::updateTransferFunction);

};

void TransferFunctionGraph::updateGraph(){
    // Remove series, and replace with new one from controll points
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
    
    // Reset Gradient
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    for(int i = 0; i < size::NumPoints; i++){
        float r = m_cmap.colorMapData()[i*size::NumChannels];
        float g = m_cmap.colorMapData()[(i*size::NumChannels)+1];
        float b = m_cmap.colorMapData()[(i*size::NumChannels)+2];
        float a = m_cmap.colorMapData()[(i*size::NumChannels)+3];

        QColor col;
        col.setRgbF(r, g, b);
        m_gradient.setColorAt(a, col);
    }

    m_areaSeries->setBrush(m_gradient);
    this->update();
    emit transferFunctionChanged(m_tfn);
};


void TransferFunctionGraph::setDisplayedColorMap(ColorMap cmap){
    m_cmap = cmap;
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    m_tfn.reset();
    updateGraph();
};


QPointF TransferFunctionGraph::mapLocalToChartPos(QPointF localpos){
    auto const scenePos = mapToScene(QPoint(static_cast<int>(localpos.x()), static_cast<int>(localpos.y()))); 
    auto const chartItemPos = chart()->mapFromScene(scenePos); 
    auto const valueGivenSeries = chart()->mapToValue(chartItemPos);
    return valueGivenSeries;
}



// When a point is clicked and held
void TransferFunctionGraph::pointPressed(const QPointF &point){
    m_currentClickedIndex = m_tfn.indexOf(point);
};

// When mouse clickes the graph
void TransferFunctionGraph::graphClicked(const QPointF &point) {
    if(m_tfn.addControlPoint(point)){
        updateGraph();
    }
}

// Gets Position of Release
void TransferFunctionGraph::mouseReleaseEvent(QMouseEvent* event){
    QGraphicsView::mouseReleaseEvent(event);
    qDebug() << "Mouse Released";

    if(m_currentClickedIndex != -1) {
        m_tfn.replace(m_currentClickedIndex, mapLocalToChartPos(event->localPos()));
        updateGraph();
        // TODO reorder controll point to avoid overlap!!
        m_currentClickedIndex = -1;
    }
}

// Moves the point while mouse is moving for better interaction
void TransferFunctionGraph::mouseMoveEvent(QMouseEvent* event){
    QGraphicsView::mouseMoveEvent(event);
    
    // TODO check if move is out of bounds!!!
    if(m_currentClickedIndex != -1){
        m_tfn.replace(m_currentClickedIndex, mapLocalToChartPos(event->localPos()));
        updateGraph();
    }
    
}


} // namespace tfn

