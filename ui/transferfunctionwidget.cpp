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

    m_layout = new QHBoxLayout();
    m_selector = new ColorMapSelector(nullptr, m_colorMapStore->availableColorMaps());
    m_tfnGraph = new TransferFunctionGraph(properties);


    connect(m_selector, &ColorMapSelector::currentTextChanged, this,
            &TransferWidget::setSelectedColorMap);
    connect(this, &TransferWidget::valueChanged,
            &m_properties.get()->transferFunction(),
            &TransferProperties::updateColorMap);
    
    m_layout->addWidget(m_selector);
    int result = m_selector->findText("gist_gray");
    result > -1 ? m_selector->setCurrentIndex(result) : setSelectedColorMap(0);

    
    m_layout->addWidget(m_tfnGraph);
    setLayout(m_layout);
};

void TransferWidget::setSelectedColorMap(const QString& name)
{
    m_selectedColorMap = name;
    m_tfnGraph->setDisplayedColorMap(m_colorMapStore->colorMap(name));
    emit valueChanged(name);
};


ColorMapSelector::ColorMapSelector(QWidget* parent,
                                   std::vector<QString> colorMaps)
    : QComboBox(parent)
{
    for (const auto& colorMap : colorMaps)
    {
        addItem(colorMap);
    }
};

} // namespace tfn
