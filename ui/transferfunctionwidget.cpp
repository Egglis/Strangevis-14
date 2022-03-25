#include "transferfunctionwidget.h"

#include "../properties/sharedproperties.h"
#include "../properties/transferproperties.h"

#include <QDebug>
#include <QFile>
#include <QtXml>

TransferWidget::TransferWidget(
    const std::shared_ptr<SharedProperties>& properties, QWidget* parent)
    : QWidget(parent), m_properties{properties}
{
    // TODO: Check if loading of xml file fails and either reload or give user
    // warning dialog
    loadColorMaps();
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
    result > -1 ? m_selector->setCurrentIndex(result)
                : setSelectedColorMap(0);
};

bool TransferWidget::loadColorMaps()
{
    // Open file for Reading
    QDomDocument xml;
    QFile f(":cmaps/res/cmaps.xml");
    if (!f.open(QIODevice::ReadOnly))
    {
        // Error:
        qDebug() << "Error while loading file";
        return false;
    }
    xml.setContent(&f);
    f.close();

    // Extract data:
    QDomElement root = xml.documentElement();
    QDomElement component = root.firstChild().toElement();
    while (!component.isNull())
    {
        if (component.tagName() == "ColorMap")
        {
            QString name = component.attribute("name");
            QString space = component.attribute("space"); // Might use later

            qDebug() << "Loading Colormap:" << name;

            // Extract RGB values and alpha values
            QDomElement child = component.firstChild().toElement();
            std::vector<GLfloat> colors{};
            colors.reserve(256);

            int index = 0;
            while (!child.isNull())
            {
                if (child.tagName() == "Point")
                {
                    float r = child.attribute("r").toFloat();
                    float g = child.attribute("g").toFloat();
                    float b = child.attribute("b").toFloat();
                    float a = child.attribute("x").toFloat();
                    colors.push_back(r);
                    colors.push_back(g);
                    colors.push_back(b);
                    colors.push_back(a);
                }
                child = child.nextSibling().toElement();
            }

            ColorMap cmap = ColorMap(name, colors);
            m_colorMaps.push_back(cmap);
        }
        component = component.nextSibling().toElement();
    }

    return true;
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
