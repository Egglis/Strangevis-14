#include "transferfunctionwidget.h"

#include "../properties/sharedproperties.h"
#include "../properties/transferproperties.h"

#include <QDebug>
#include <QFile>
#include <QtXml>

TransferWidget::TransferWidget(
    const std::shared_ptr<ISharedProperties> properties, QWidget* parent)
    : QWidget(parent), m_properties{properties}
{
    // TODO: Check if loading of xml file fails and either reload or give user
    // warning dialog
    m_colorMaps = loadColorMapsFromFile();
    m_layout = new QHBoxLayout();
    m_selector = new ColorMapSelector(nullptr, m_colorMaps);
    connect(m_selector, &ColorMapSelector::currentIndexChanged, this,
            &TransferWidget::setSelectedColorMap);
    connect(this, &TransferWidget::valueChanged,
            &m_properties.get()->colorMap(),
            &TransferProperties::transferFunctionChanged);
    m_layout->addWidget(m_selector);
    setLayout(m_layout);
    int result = m_selector->findText("gist_gray");
    result > -1 ? m_selector->setCurrentIndex(result) : setSelectedColorMap(0);
};

void TransferWidget::setSelectedColorMap(int index)
{
    m_index = index;
    emit valueChanged(m_colorMaps[m_index]);
};

ColorMapSelector::ColorMapSelector(QWidget* parent,
                                   std::vector<ColorMap> colorMaps)
    : QComboBox(parent)
{
    for (const auto& colorMap : colorMaps)
    {
        addItem(colorMap.getName());
    }
}
