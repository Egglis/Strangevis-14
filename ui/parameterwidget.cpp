#include "parameterwidget.h"

#include "../properties/sharedproperties.h"

#include <algorithm>

ParameterWidget::ParameterWidget(
    const std::shared_ptr<SharedProperties>& properties, QWidget* parent)
    : QWidget(parent), m_properties{properties}
{

    ClippingWidget* x = new ClippingWidget(this);
    m_clippingPlaneWidgets.push_back(x);
    x->setValue(m_properties->clippingPlane().x());
    connect(x, &ClippingWidget::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(m_properties.get(), &SharedProperties::clippingPlaneChanged,
            [this, x](const QVector4D& clippingPlane) {
                x->setValue(clippingPlane.x());
            });
    m_layout.addWidget(x);

    ClippingWidget* y = new ClippingWidget(this);
    m_clippingPlaneWidgets.push_back(y);
    y->setValue(m_properties->clippingPlane().y());
    connect(y, &ClippingWidget::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(m_properties.get(), &SharedProperties::clippingPlaneChanged,
            [this, y](const QVector4D& clippingPlane) {
                y->setValue(clippingPlane.y());
            });
    m_layout.addWidget(y);

    ClippingWidget* z = new ClippingWidget(this);
    m_clippingPlaneWidgets.push_back(z);
    z->setValue(m_properties->clippingPlane().z());
    connect(z, &ClippingWidget::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(m_properties.get(), &SharedProperties::clippingPlaneChanged,
            [this, z](const QVector4D& clippingPlane) {
                z->setValue(clippingPlane.z());
            });
    m_layout.addWidget(z);

    ClippingWidget* w = new ClippingWidget(this);
    m_clippingPlaneWidgets.push_back(w);
    w->setValue(m_properties->clippingPlane().w());
    connect(w, &ClippingWidget::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(m_properties.get(), &SharedProperties::clippingPlaneChanged,
            [this, w](const QVector4D& clippingPlane) {
                w->setValue(clippingPlane.w());
            });
    m_layout.addWidget(w);

    m_gradientMethodWidget = new GradientMethodWidget(this);
    m_layout.addWidget(m_gradientMethodWidget);
    connect(m_gradientMethodWidget, &GradientMethodWidget::valueChanged,
            m_properties.get(), &SharedProperties::updateGradientMethod);
    m_gradientMethodWidget->setValue(m_properties->gradientMethod());

    setLayout(&m_layout);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
}

float ClippingWidget::intToFloat(int value)
{
    float t = static_cast<float>((value - m_sliderMinimum)) /
              static_cast<float>((m_sliderMaximum - m_sliderMinimum));
    float newValue = (1 - t) * m_lowerBound + t * m_upperBound;
    return newValue;
}

int ClippingWidget::floatToInt(float value)
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
        clippingPlane[i] = m_clippingPlaneWidgets[i]->getValue();
    }
    m_properties->updateClippingPlane(clippingPlane);
}

GradientMethodWidget::GradientMethodWidget(QWidget* parent)
    : QWidget(parent), m_layout{this}
{
    m_gradientMethodLabel = new QLabel(this);
    m_gradientMethodSlider = new QSlider(Qt::Orientation::Horizontal, this);
    m_layout.addRow(m_gradientMethodLabel, m_gradientMethodSlider);

    m_gradientMethodSlider->setRange(0, 2);
    connect(m_gradientMethodSlider, &QSlider::valueChanged, this,
            &GradientMethodWidget::setValue);
}

void GradientMethodWidget::updateLabel(int value)
{
    switch (value)
    {
    case 0:
        m_gradientMethodLabel->setText("Central Difference");
        break;
    case 1:
        m_gradientMethodLabel->setText("Forward Difference");
        break;
    case 2:
        m_gradientMethodLabel->setText("Backward Difference");
        break;
    default:
        m_gradientMethodLabel->setText("Invalid value");
        break;
    }
}

void GradientMethodWidget::setValue(int value)
{
    m_gradientMethodSlider->setValue(value);
    updateLabel(value);
    emit valueChanged(value);
}

ClippingWidget::ClippingWidget(QWidget* parent) : QWidget(parent)
{
    m_slider = new QSlider(Qt::Orientation::Horizontal, this);
    m_slider->setMinimum(0);
    m_slider->setMaximum(100);

    m_label = new QLabel(this);
    auto updateLabel = [this](int value) {
        QString s;
        s.setNum(intToFloat(value), 'f', 2);
        m_label->setText(s);
    };
    connect(m_slider, &QSlider::valueChanged, updateLabel);
    m_layout.addRow(m_label, m_slider);
    connect(m_slider, &QSlider::valueChanged, this,
            &ClippingWidget::valueChanged);
    setLayout(&m_layout);
}

void ClippingWidget::setValue(float value)
{
    m_slider->setValue(floatToInt(value));
}
