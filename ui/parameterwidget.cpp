#include "parameterwidget.h"

#include "../properties/clippingplaneproperties.h"
#include "../properties/gradientproperties.h"
#include "../properties/sharedproperties.h"

ParameterWidget::ParameterWidget(
    const std::shared_ptr<SharedProperties>& properties, QWidget* parent)
    : QWidget(parent), m_properties{properties}, m_layout{this}
{
    ClippingWidget* x = new ClippingWidget(nullptr);
    m_clippingPlaneWidgets.push_back(x);
    x->setValue(m_properties->clippingPlane().plane().normal().x());
    connect(x, &ClippingWidget::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged,
            [this, x](const Plane& clippingPlane) {
                x->setValue(clippingPlane.normal().x());
            });
    m_layout.addWidget(x);

    ClippingWidget* y = new ClippingWidget(nullptr);
    m_clippingPlaneWidgets.push_back(y);
    y->setValue(m_properties->clippingPlane().plane().normal().y());
    connect(y, &ClippingWidget::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged,
            [this, y](const Plane& clippingPlane) {
                y->setValue(clippingPlane.normal().y());
            });
    m_layout.addWidget(y);

    ClippingWidget* z = new ClippingWidget(nullptr);
    m_clippingPlaneWidgets.push_back(z);
    z->setValue(m_properties->clippingPlane().plane().normal().z());
    connect(z, &ClippingWidget::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged,
            [this, z](const Plane& clippingPlane) {
                z->setValue(clippingPlane.normal().z());
            });
    m_layout.addWidget(z);

    ClippingWidget* w = new ClippingWidget(nullptr);
    m_clippingPlaneWidgets.push_back(w);
    w->setValue(m_properties->clippingPlane().plane().d());
    connect(w, &ClippingWidget::valueChanged, this,
            &ParameterWidget::updateClippingPlane);
    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged,
            [this, w](const Plane& clippingPlane) {
                w->setValue(clippingPlane.d());
            });
    m_layout.addWidget(w);

    m_gradientMethodWidget = new GradientMethodWidget(nullptr);
    connect(m_gradientMethodWidget, &GradientMethodWidget::valueChanged,
            &m_properties.get()->gradientMethod(),
            &GradientProperties::updateGradientMethod);
    m_gradientMethodWidget->setValue(m_properties->gradientMethod().method());
    m_layout.addWidget(m_gradientMethodWidget);

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
    Plane plane = Plane(clippingPlane);
    m_properties->clippingPlane().updateClippingPlane(plane);
}

GradientMethodWidget::GradientMethodWidget(QWidget* parent)
    : QWidget(parent), m_layout{this}
{
    m_gradientMethodLabel = new QLabel(this);
    m_gradientMethodSlider = new QSlider(Qt::Orientation::Horizontal, this);
    m_layout.addRow(m_gradientMethodLabel, m_gradientMethodSlider);

    m_gradientMethodSlider->setRange(0, 2);
    connect(m_gradientMethodSlider, &QSlider::valueChanged, this,
            qOverload<int>(&GradientMethodWidget::setValue));
}

void GradientMethodWidget::updateLabel(GradientMethod method)
{
    switch (method)
    {
    default:
        m_gradientMethodLabel->setText("Invalid value");
        break;
    case GradientMethod::CentralDifference:
        m_gradientMethodLabel->setText("Central Difference");
        break;
    case GradientMethod::ForwardDifference:
        m_gradientMethodLabel->setText("Forward Difference");
        break;
    case GradientMethod::BackwardsDifference:
        m_gradientMethodLabel->setText("Backward Difference");
        break;
    }
}

void GradientMethodWidget::setValue(GradientMethod method)
{
    m_gradientMethodSlider->setValue(static_cast<int>(method));
    updateLabel(method);
    emit valueChanged(method);
}

void GradientMethodWidget::setValue(int value)
{
    m_gradientMethodSlider->setValue(value);
    GradientMethod method = static_cast<GradientMethod>(value);
    updateLabel(method);
    emit valueChanged(method);
}

ClippingWidget::ClippingWidget(QWidget* parent)
    : QWidget(parent), m_layout{this}
{
    m_slider = new QSlider(Qt::Orientation::Horizontal, this);
    m_slider->setMinimum(m_sliderMinimum);
    m_slider->setMaximum(m_sliderMaximum);

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
}

void ClippingWidget::setValue(float value)
{
    m_slider->setValue(floatToInt(value));
}
