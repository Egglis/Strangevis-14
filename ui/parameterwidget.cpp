#include "parameterwidget.h"

#include "../properties/sharedproperties.h"

#include <algorithm>

ParameterWidget::ParameterWidget(
    const std::shared_ptr<SharedProperties>& properties, QWidget* parent)
    : QWidget(parent), m_properties{properties}
{
    QSlider* x = createSlider();
    clippingPlaneSliders.push_back(x);
    layout.addWidget(x);
    x->setValue(floatToInt(m_properties->clippingPlane().x()));
    connect(x, &QSlider::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(m_properties.get(), &SharedProperties::clippingPlaneChanged,
            [this, x](const QVector4D& clippingPlane) {
                x->setValue(floatToInt(clippingPlane.x()));
            });
    createSliderLabel(x);


    QSlider* y = createSlider();
    clippingPlaneSliders.push_back(y);
    layout.addWidget(y);
    y->setValue(floatToInt(m_properties->clippingPlane().y()));
    connect(y, &QSlider::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(m_properties.get(), &SharedProperties::clippingPlaneChanged,
            [this, y](const QVector4D& clippingPlane) {
                y->setValue(floatToInt(clippingPlane.y()));
            });
    createSliderLabel(y);


    QSlider* z = createSlider();
    clippingPlaneSliders.push_back(z);
    layout.addWidget(z);
    z->setValue(floatToInt(m_properties->clippingPlane().z()));
    connect(z, &QSlider::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(m_properties.get(), &SharedProperties::clippingPlaneChanged,
            [this, z](const QVector4D& clippingPlane) {
                z->setValue(floatToInt(clippingPlane.z()));
            });
    createSliderLabel(z);

    QSlider* w = createSlider();
    clippingPlaneSliders.push_back(w);
    layout.addWidget(w);
    w->setValue(floatToInt(m_properties->clippingPlane().w()));
    connect(w, &QSlider::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(m_properties.get(), &SharedProperties::clippingPlaneChanged,
            [this, w](const QVector4D& clippingPlane) {
                w->setValue(floatToInt(clippingPlane.w()));
            });
    createSliderLabel(w);

    setLayout(&layout);
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
}

QSlider* ParameterWidget::createSlider()
{
    QSlider* slider = new QSlider(Qt::Orientation::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    return slider;
}

void ParameterWidget::createSliderLabel(QSlider* slider)
{
    QLabel* label = new QLabel(this);
    auto updateLabel = [this, label](int value) { label->setNum(intToFloat(value));};
    connect(slider, &QSlider::valueChanged,
            updateLabel);
    sliderLabels.push_back(label);
    layout.addWidget(label);
    updateLabel(slider->value());
}

float ParameterWidget::intToFloat(int value)
{
    float t = static_cast<float>((value - m_sliderMinimum)) /
              static_cast<float>((m_sliderMaximum - m_sliderMinimum));
    float newValue = (1 - t) * m_lowerBound + t * m_upperBound;
    return newValue;
}

int ParameterWidget::floatToInt(float value)
{
    float t = (value - m_lowerBound) / (m_upperBound - m_lowerBound);
    int newValue =
        static_cast<int>((1 - t) * m_sliderMinimum + t * m_sliderMaximum);
    return newValue;
}

void ParameterWidget::updateClippingPlane()
{
    QVector4D clippingPlane = QVector4D();

    for (int i = 0; i < 4; i++)
    {
        clippingPlane[i] = intToFloat(clippingPlaneSliders[i]->value());
    }
    m_properties->updateClippingPlane(clippingPlane);
}
