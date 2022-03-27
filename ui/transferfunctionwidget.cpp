#include "transferfunctionwidget.h"

#include "../properties/sharedproperties.h"
#include "../properties/transferproperties.h"

#include <QDebug>

namespace tfn
{

// Main Transfer Function widget
TransferWidget::TransferWidget(
    const std::shared_ptr<ISharedProperties> properties, const std::shared_ptr<const IColorMapStore> colorMapStore, QWidget* parent)
    : QWidget(parent), m_properties{properties}, m_colorMapStore{colorMapStore}
{


    // TODO: Check if loading of xml file fails and either reload or give user
    // warning dialog
    m_layout = new QHBoxLayout();
    m_selector = new ColorMapSelector(nullptr, m_colorMapStore->availableColorMaps());

    connect(m_selector, &ColorMapSelector::currentTextChanged, this,
            &TransferWidget::setSelectedColorMap);
    connect(this, &TransferWidget::valueChanged,
            &m_properties.get()->colorMap(),
            &TransferProperties::updateTexture);
    
    m_layout->addWidget(m_selector);
    int result = m_selector->findText("Oranges");
    result > -1 ? m_selector->setCurrentIndex(result) : setSelectedColorMap(0);

    m_colorMapChart = new ColorMapChart(m_colorMapStore->colorMap(m_selectedColorMap));
    m_layout->addWidget(m_colorMapChart);
    setLayout(m_layout);
};

void TransferWidget::setSelectedColorMap(const QString& name)
{
    m_selectedColorMap = name;
    emit valueChanged(name);
};

// Color Map Selector 
ColorMapSelector::ColorMapSelector(QWidget* parent,
                                   std::vector<QString> colorMaps)
    : QComboBox(parent)
{
    for (const auto& colorMap : colorMaps)
    {
        addItem(colorMap);
    }
};

// Color Map Chart 
ColorMapChart::ColorMapChart(ColorMap cmap) : m_currentColorMap(cmap) {

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
    this->setFixedSize(300, 300);

    updateChart();
};

void ColorMapChart::setColorMap(ColorMap cmap){
    qDebug() << cmap.getName();
    m_currentColorMap = cmap;
    updateChart();
};

void ColorMapChart::updateChart(){
    for(int i = 0; i < 256; i++){
        float r = m_currentColorMap.colorMapData()[i*3];
        float g = m_currentColorMap.colorMapData()[(i*3)+1];
        float b = m_currentColorMap.colorMapData()[(i*3)+2];
        float a = (float) i/ (float) 256;

        QColor col;
        col.setRgbF(r, g, b);
        m_gradient.setColorAt(a, col);
    }

    m_areaSeries->setBrush(m_gradient);
    this->update();
};

} // namespace tfn