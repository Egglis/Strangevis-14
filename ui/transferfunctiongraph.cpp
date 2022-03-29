#include "transferfunctiongraph.h"

namespace tfn {


TransferFunctionGraph::TransferFunctionGraph(){

    // Defualt Controll Points
    // TODO CHANGE
    m_controllPoints.push_back(QPointF(0,0));
    m_controllPoints.push_back(QPointF(255, 1));


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
    this->setFixedSize(600, 600); // TODO Temporary


    connect(m_scatterSeries, &QScatterSeries::pressed,
            this, &TransferFunctionGraph::pointPressed); 

    connect(m_boundingBox, &QAreaSeries::pressed,
            this, &TransferFunctionGraph::graphClicked);
};

void TransferFunctionGraph::updateGraph(){
    // Remove series, and replace with new one from controll points
    m_chart->removeSeries(m_areaSeries);
    m_chart->removeSeries(m_scatterSeries);
    m_chart->removeSeries(m_boundingBox);

    m_lineSeries->clear();
    m_lineSeries->append(m_controllPoints);

    m_scatterSeries->clear();
    m_scatterSeries->append(m_controllPoints);

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
        col.setRgbF(r, g, b, a);
        m_gradient.setColorAt(a, col);
    }

    m_areaSeries->setBrush(m_gradient);
    this->update();
    applyColorMapChanges();
};


void TransferFunctionGraph::applyColorMapChanges(){
    QString name = "Custome1";

    std::vector<GLfloat> colors{};
    colors.reserve(size::ArraySize);

    int from_x = 0; // From x
    for(int i = 0; i < m_controllPoints.length()-1; i++){
        int target_x = (int) m_controllPoints[i+1].x(); // Target x
        for(int t = from_x; t <= target_x; t++){
            auto r = m_cmap.colorMapData()[t*size::NumChannels];
            auto g = m_cmap.colorMapData()[(t*size::NumChannels)+1];
            auto b = m_cmap.colorMapData()[(t*size::NumChannels)+2];
            auto a = solveSlopeEquation(m_controllPoints[i], m_controllPoints[i+1], t);
            colors.push_back(r);
            colors.push_back(g);
            colors.push_back(b);
            colors.push_back(a);   
        }
        from_x = target_x+1;
    }
    assert(colors.size() == size::ArraySize);
    ColorMap cmap = ColorMap(name, colors);
    m_cmap = cmap;
    // TODO emit colormap changed and create new custome colormap???
}


void TransferFunctionGraph::setDisplayedColorMap(ColorMap cmap){
    m_cmap = cmap;
    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    m_controllPoints.clear();
    m_controllPoints.push_back(QPointF(0,0));
    m_controllPoints.push_back(QPointF(255, 1));
    updateGraph();
};

bool TransferFunctionGraph::addControllPoint(QPointF pos){
    // Bounds Checking?
    if (pos.x() <= 0 || pos.x() >= 255 || pos.y() <= 0 || pos.y() >= 1) {
        return false;
    }
    for(int i = 0; i < m_controllPoints.length(); i++){
        if(m_controllPoints[i].x() < pos.x() && pos.x() < m_controllPoints[i+1].x()){
            m_controllPoints.insert(i+1, pos);
            break;
        }
    }
    return true;
};

QPointF TransferFunctionGraph::mapLocalToChartPos(QPointF localpos){
    auto const scenePos = mapToScene(QPoint(static_cast<int>(localpos.x()), static_cast<int>(localpos.y()))); 
    auto const chartItemPos = chart()->mapFromScene(scenePos); 
    auto const valueGivenSeries = chart()->mapToValue(chartItemPos);
    return valueGivenSeries;
}

float TransferFunctionGraph::solveSlopeEquation(QPointF p0, QPointF p1, int t){
    float m = (p1.y() - p0.y())/(p1.x() - p0.x());
    return m*(t-p0.x())+p0.y();
};

// When a point is clicked and held
void TransferFunctionGraph::pointPressed(const QPointF &point){
    qDebug() << "Point Pressed: ";
    qDebug() << "Point Pos:" << point;
    qDebug() << "Setting Current Index:" << m_controllPoints.indexOf(point);
    m_currentClickedIndex = m_controllPoints.indexOf(point);
};

// When mouse clickes the graph
void TransferFunctionGraph::graphClicked(const QPointF &point) {
    if(addControllPoint(point)){
        updateGraph();
    }
}

// Gets Position of Release
void TransferFunctionGraph::mouseReleaseEvent(QMouseEvent* event){
    QGraphicsView::mouseReleaseEvent(event);
    qDebug() << "Mouse Released";

    if(m_currentClickedIndex != -1) {
        m_controllPoints.replace(m_currentClickedIndex, mapLocalToChartPos(event->localPos()));
        updateGraph();
        // TODO reorder controll point to avoid overlap??
        m_currentClickedIndex = -1;
    }
}

// Moves the point while mouse is moving for better interaction
void TransferFunctionGraph::mouseMoveEvent(QMouseEvent* event){
    QGraphicsView::mouseMoveEvent(event);

    /*
    qDebug() << solveSlopeEquation(m_controllPoints[0], m_controllPoints[1], m_test);
    m_test = m_test + 1;
    */


    if(m_currentClickedIndex != -1){
        m_controllPoints.replace(m_currentClickedIndex, mapLocalToChartPos(event->localPos()));
        updateGraph();
    }
    
}


} // namespace tfn

