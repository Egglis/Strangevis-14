#include "transferfunctiongraph.h"

namespace tfn {


TransferFunctionGraph::TransferFunctionGraph(){

    m_controllPoints.push_back(QPointF(0,0));
    m_controllPoints.push_back(QPointF(255, 1));

    m_chart = new QChart();
    m_chart->legend()->hide();
    
    m_pen = new QPen(0xa5a5a4);
    m_pen->setWidth(3);

    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    m_lineSeries = new QLineSeries();
    // m_lineSeries->append(m_controllPoints);

    m_scatterSeries = new QScatterSeries();
    m_scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_scatterSeries->setMarkerSize(15);
    m_scatterSeries->setBorderColor(QColor(0,0,0));
    m_scatterSeries->setColor(QColor(150,150,150,0.4));

    m_areaSeries = new QAreaSeries();
    m_areaSeries->setName("Gradient Display");
    m_areaSeries->setPen(*m_pen);

    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_scatterSeries);
    m_chart->createDefaultAxes();
    m_chart->setTitle("Test Graph");

    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setFixedSize(600, 600); // TODO Temporary


    connect(m_scatterSeries, &QScatterSeries::clicked,
           this, &TransferFunctionGraph::pointClicked); // Public slot 

};

void TransferFunctionGraph::updateGraph(){
    // Remove series, and replace with new one from controll points
    m_chart->removeSeries(m_areaSeries);
    m_chart->removeSeries(m_scatterSeries);

    m_lineSeries->clear();
    m_lineSeries->append(m_controllPoints);

    m_scatterSeries->clear();
    m_scatterSeries->append(m_controllPoints);

    m_areaSeries->setUpperSeries(m_lineSeries);
    m_chart->addSeries(m_areaSeries);
    m_chart->addSeries(m_scatterSeries);

    for(int i = 0; i < 256; i++){
        float r = m_cmap.colorMapData()[i*3];
        float g = m_cmap.colorMapData()[(i*3)+1];
        float b = m_cmap.colorMapData()[(i*3)+2];
        float a = (float) i/ (float) 256;

        QColor col;
        col.setRgbF(r, g, b);
        m_gradient.setColorAt(a, col);
    }

    m_areaSeries->setBrush(m_gradient);
    this->update();
};

void TransferFunctionGraph::setDisplayedColorMap(ColorMap cmap){
    m_cmap = cmap;
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

void TransferFunctionGraph::pointClicked(const QPointF &point){
    qDebug() << "Point Clicked";
    qDebug() << point;
};

void TransferFunctionGraph::mousePressEvent(QMouseEvent* p_event){
    if(p_event->button()==Qt::LeftButton){
        qDebug() << "Graph Pressed";
        
        // Transforms widget position into graph values:
        auto const widgetPos = p_event->localPos();
        auto const scenePos = mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y()))); 
        auto const chartItemPos = chart()->mapFromScene(scenePos); 
        auto const valueGivenSeries = chart()->mapToValue(chartItemPos); 
        /*
        qDebug() << "widgetPos:" << widgetPos; 
        qDebug() << "scenePos:" << scenePos; 
        qDebug() << "chartItemPos:" << chartItemPos; 
        qDebug() << "valSeries:" << valueGivenSeries;
        */
        // Replace Clicked points
        // Add new Controll points

        if(addControllPoint(valueGivenSeries)){
            updateGraph();
            //qDebug() << m_controllPoints;
        }
    }


};

void TransferFunctionGraph::mouseMoveEvent(QMouseEvent* p_event){

};

void TransferFunctionGraph::mouseReleaseEvent(QMouseEvent* p_event){

};


} // namespace tfn

