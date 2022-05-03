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
        new ColorMapSelector(nullptr, m_colorMapStore->availableColorMaps());
    m_tfnGraph = new TransferFunctionGraph(properties);

    connect(m_selector, &ColorMapSelector::currentTextChanged, this,
            &TransferWidget::setSelectedColorMap);
    connect(this, &TransferWidget::valueChanged,
            &m_properties.get()->transferFunction(),
            &TransferProperties::updateColorMap);

    setSelectedColorMap(0);

    v_layout->addWidget(m_selector,4);
    auto* button_layout = new QHBoxLayout();
    v_layout->addLayout(button_layout,1);
    auto* button = new QPushButton("Reset");

    connect(button, &QPushButton::pressed, m_tfnGraph, &TransferFunctionGraph::reset);

    auto checkbox = new QCheckBox("Overlay Histogram");
    button_layout->addWidget(button);
    button_layout->addWidget(checkbox);
    m_layout->addWidget(m_tfnGraph, 3);
    m_layout->addLayout(v_layout, 1);
    setLayout(m_layout);

    connect(checkbox, &QRadioButton::toggled, m_tfnGraph, &TransferFunctionGraph::setVisibleHistogram);
};

void TransferWidget::setSelectedColorMap(const QString& name)
{
    m_selectedColorMap = name;
    m_tfnGraph->setDisplayedColorMap(m_colorMapStore->colorMap(name));
    emit valueChanged(name);
};

ColorMapSelector::ColorMapSelector(QWidget* parent,
                                   std::vector<QString> colorMaps)
    : QListWidget(parent)
{
    for (const auto& colorMap : colorMaps)
    {
        addItem(colorMap);
    }
};

} // namespace tfn
