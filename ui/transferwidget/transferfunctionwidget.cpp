#include "transferfunctionwidget.h"

#include "../properties/sharedproperties.h"
#include "../properties/transferproperties.h"

#include <QDebug>

namespace tfn
{

// Main Transfer Function widget
TransferWidget::TransferWidget(
    const std::shared_ptr<ISharedProperties> properties,
    const std::shared_ptr<const IColorMapStore> colorMapStore, QWidget* parent)
    : QWidget(parent), m_properties{properties}, m_colorMapStore{colorMapStore}
{

    m_layout = new QHBoxLayout();
    auto* v_layout = new QVBoxLayout();
    m_selector =
        new ColorMapSelector(nullptr, m_colorMapStore);
    m_tfnGraph = new TransferFunctionGraph(properties);

    connect(m_selector, &ColorMapSelector::currentTextChanged, this,
            &TransferWidget::setSelectedColorMap);
    connect(this, &TransferWidget::valueChanged,
            &m_properties.get()->transferFunction(),
            &TransferProperties::updateColorMap);

    setSelectedColorMap(0);

    v_layout->addWidget(m_selector, 4);
    auto* button_layout = new QHBoxLayout();
    v_layout->addLayout(button_layout, 1);
    auto* button = new QPushButton("Reset");

    connect(button, &QPushButton::pressed, m_tfnGraph,
            &TransferFunctionGraph::reset);

    QSlider* histogramSlider = new QSlider(Qt::Vertical, this);
    m_tfnGraph->setHistogramSlider(histogramSlider);
    connect(histogramSlider, &QSlider::valueChanged, m_tfnGraph, &TransferFunctionGraph::setHistogramScaling);

    auto checkbox = new QCheckBox("Overlay Histogram");
    button_layout->addWidget(button);
    button_layout->addWidget(checkbox);
    m_layout->addWidget(histogramSlider, 1);
    m_layout->addWidget(m_tfnGraph, 5);
    m_layout->addLayout(v_layout, 2);
    setLayout(m_layout);

    connect(checkbox, &QRadioButton::toggled, m_tfnGraph, &TransferFunctionGraph::setVisibleHistogram);
};

void TransferWidget::setSelectedColorMap(const QString& name)
{
    m_selectedColorMap = name;
    m_tfnGraph->setDisplayedColorMap(m_colorMapStore->colorMap(name));
    emit valueChanged(name);
};

ColorMapSelector::ColorMapSelector(
    QWidget* parent, const std::shared_ptr<const IColorMapStore> colorMapStore)
    : QListWidget(parent)
{
    for (const auto& colorMapName : colorMapStore->availableColorMaps())
    {
        QListWidgetItem* item = new QListWidgetItem(colorMapName);
        QLinearGradient gradient =
            QLinearGradient(QPointF(0, 0), QPointF(1, 0));
        gradient.setCoordinateMode(QGradient::ObjectBoundingMode);

        const auto colorMapData =
            colorMapStore->colorMap(colorMapName).colorMapData();
        for (int i = 0; i < tfn::size::NUM_POINTS; i++)
        {
            float r = colorMapData.at(i * tfn::size::NUM_CHANNELS);
            float g = colorMapData.at(i * tfn::size::NUM_CHANNELS + 1);
            float b = colorMapData.at(i * tfn::size::NUM_CHANNELS + 2);

            QColor col;
            col.setRgbF(r, g, b, 0.8f);
            gradient.setColorAt(i / static_cast<float>(tfn::size::NUM_POINTS),
                                col);
        }
        QBrush brush = QBrush(gradient);

        item->setBackground(brush);
        addItem(item);
    }
};
} // namespace tfn
