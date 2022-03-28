#include "transferfunctiongraph.h"

namespace tfn {


TransferFunctionGraph::TransferFunctionGraph(){
    m_chart = new QChart();
    m_chart->legend()->hide();
    
    m_pen = new QPen(0xa5a5a4);
    m_pen->setWidth(3);

    m_gradient = QLinearGradient(QPointF(0, 0), QPointF(1, 0));
    m_gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

    m_lineSeries = new QLineSeries();
    m_lineSeries->append(QPointF(0,0));
    m_lineSeries->append(QPointF(255, 1));

    m_areaSeries = new QAreaSeries(m_lineSeries);
    m_areaSeries->setName("Gradient Display");
    m_areaSeries->setPen(*m_pen);

    m_chart->addSeries(m_areaSeries);
    m_chart->createDefaultAxes();
    m_chart->setTitle("Test Graph");

    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);
    this->setFixedSize(300, 300); // TODO Temporary 
};

void TransferFunctionGraph::updateGraph(){
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


} // namespace tfn

