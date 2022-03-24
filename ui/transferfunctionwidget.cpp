#include "transferfunctionwidget.h"

#include "../properties/sharedproperties.h"
#include "../properties/transferproperties.h"

#include <QDebug>
#include <QtXml>
#include <QFile>

TransferWidget::TransferWidget(const std::shared_ptr<SharedProperties>& properties, QWidget* parent) : QWidget(parent), m_properties{properties} {
    // TODO: Check if loading of xml file fails and either reload or give user warning dialog
    bool load = loadColorMaps();

    m_selector = new ColorMapSelector(nullptr, m_colorMaps);
    connect(m_selector, &ColorMapSelector::valueChanged,
            this, &TransferWidget::setSelectedColorMap);
    connect(this, &TransferWidget::valueChanged,
        &m_properties.get()->colorMap(), &TransferProperties::transferTextureChanged);
    m_layout.addWidget(m_selector);
    setLayout(&m_layout);
};

bool TransferWidget::loadColorMaps(){
    // Open file for Reading
    QDomDocument xml;
    QFile f(":cmaps/res/cmaps.xml");
        // Error:
        qDebug() << "Error while loading file";
        return false;
    }
    xml.setContent(&f);
    f.close();

    // Extract data:
    QDomElement root = xml.documentElement();
    QDomElement component = root.firstChild().toElement();
    while(!component.isNull()){
        if(component.tagName() == "ColorMap"){
            QString name = component.attribute("name");
            QString space = component.attribute("space"); // Might use later
            
            qDebug() << "Loading Colormap:" << name;

            // Extract RGB values and alpha values
            QDomElement child = component.firstChild().toElement();
            GLfloat colors[256][4];
 
            int index = 0;
            while(!child.isNull()){
                if(child.tagName() == "Point"){
                    float r = child.attribute("r").toFloat();
                    float g = child.attribute("g").toFloat();
                    float b = child.attribute("b").toFloat();
                    float a = child.attribute("x").toFloat();
                    colors[index][0] = r;
                    colors[index][1] = g;
                    colors[index][2] = b;
                    colors[index][3] = a;
                }
                index = index + 1;
                child = child.nextSibling().toElement();
            }

            ColorMap cmap = ColorMap(name, colors);
            m_colorMaps.push_back(cmap);
        }
        component = component.nextSibling().toElement();
    }


    return true;
};

void TransferWidget::setSelectedColorMap(int index){
    m_index = index;
    emit valueChanged(m_colorMaps[m_index]);
};


ColorMapSelector::ColorMapSelector(QWidget* parent, std::vector<ColorMap> colorMaps) : QWidget(parent) {
    m_box = new QComboBox(this);
    for(int i = 0; i < colorMaps.size(); i++){
        m_box->addItem(colorMaps[i].getName());
    }
    connect(m_box, &QComboBox::currentIndexChanged, this,
            &ColorMapSelector::setIndex);
};

void ColorMapSelector::setIndex(int index){
    m_selectedItem = index;
    emit valueChanged(m_selectedItem);
};

