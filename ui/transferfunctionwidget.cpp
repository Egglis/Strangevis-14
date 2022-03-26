#include "transferfunctionwidget.h"

#include "../properties/sharedproperties.h"
#include "../properties/transferproperties.h"

#include <QDebug>
namespace tfn
{

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
    setLayout(m_layout);
    int result = m_selector->findText("gist_gray");
    result > -1 ? m_selector->setCurrentIndex(result) : setSelectedColorMap(0);
};

void TransferWidget::setSelectedColorMap(const QString& name)
{
    m_selectedColorMap = name;
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
}

} // namespace tfn
