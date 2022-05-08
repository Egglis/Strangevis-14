#include "parameterwidget.h"

#include "../properties/clippingplaneproperties.h"
#include "../properties/gradientproperties.h"
#include "../properties/sharedproperties.h"

ParameterWidget::ParameterWidget(
    const std::shared_ptr<ISharedProperties> properties, QWidget* parent)
    : QWidget(parent), m_properties{properties}, m_layout{this}
{
    m_gradientMethodWidget = new GradientMethodWidget(nullptr);
    connect(m_gradientMethodWidget, &GradientMethodWidget::valueChanged,
            &m_properties.get()->gradientMethod(),
            &GradientProperties::updateGradientMethod);
    m_gradientMethodWidget->setValue(m_properties->gradientMethod().method());
    m_layout.addWidget(m_gradientMethodWidget);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
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
