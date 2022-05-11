#include "rendersettingswidget.h"

RenderSettingsWidget::RenderSettingsWidget(
    const std::shared_ptr<ISharedProperties> properties, QWidget* parent)
    : m_properties{properties}, QWidget(parent)
{
    m_renderSettings = {};
    m_layout = new QVBoxLayout();

    setupSettings();
    setupWidgets();
    updateRenderSettings();

    this->setLayout(m_layout);
}

void RenderSettingsWidget::setupSettings()
{
    m_floatSliders.insert("ambientInt",
                          new FloatSlider("Ambient Intensity:", 0.1f));
    m_floatSliders.insert("diffuseInt",
                          new FloatSlider("Diffuse Intensity:", 1.5f));
    m_floatSliders.insert("specInt",
                          new FloatSlider("Specular Intensity:", 1.0f));
    m_floatSliders.insert("specCoeff",
                          new FloatSlider("Specular Coefficient:", 60.0f));

    m_floatSliders["specCoeff"]->setBounds(0.0f, 100.0f);
    m_floatSliders["specCoeff"]->setValue(60.0f);

    m_boolCheckboxes.insert("specOff", new BoolCheckbox("Specular Highlights:", true));
    m_boolCheckboxes.insert("maxInt", new BoolCheckbox("Maximum intensity projection:", false));
    m_boolCheckboxes.insert("headLight", new BoolCheckbox("Use Head Light:", false));
    m_boolCheckboxes.insert("hideSlice", new BoolCheckbox("Hide Slice:", false));

    m_intSliders.insert("stepSize", new IntSlider("Nr Slices:", 257));
    m_intSliders["stepSize"]->setSliderBounds(0, 1000);
    m_intSliders["stepSize"]->setValue(257);
};

void RenderSettingsWidget::setupWidgets()
{

    QList<QWidget*> order =
        QList<QWidget*>(Settings::SETTINGS_ORDER.length(), new QWidget());
    for (auto [key, w] : m_boolCheckboxes.toStdMap())
    {
        connect(w, &BoolCheckbox::valueChanged, this,
                &RenderSettingsWidget::updateRenderSettings);
        order.replace(Settings::SETTINGS_ORDER.indexOf(key),
                      static_cast<QWidget*>(w));
    }
    for (auto [key, w] : m_floatSliders.toStdMap())
    {
        connect(w, &SliderWidget::valueChanged, this,
                &RenderSettingsWidget::updateRenderSettings);
        order.replace(Settings::SETTINGS_ORDER.indexOf(key),
                      static_cast<QWidget*>(w));
    }
    for (auto [key, w] : m_intSliders.toStdMap())
    {
        connect(w, &SliderWidget::valueChanged, this,
                &RenderSettingsWidget::updateRenderSettings);
        order.replace(Settings::SETTINGS_ORDER.indexOf(key),
                      static_cast<QWidget*>(w));
    }

    for (auto w : order)
    {
        m_layout->addWidget(w);
    }
}

void RenderSettingsWidget::updateRenderSettings()
{
    for (auto s : m_floatSliders.keys())
    {
        m_renderSettings[s] = m_floatSliders[s]->getValue();
    }
    for (auto s : m_boolCheckboxes.keys())
    {
        m_renderSettings[s] = m_boolCheckboxes[s]->getValue();
    }
    for (auto s : m_intSliders.keys())
    {
        m_renderSettings[s] = m_intSliders[s]->getValue();
    }
    m_properties->renderSettings().updateRenderSettings(m_renderSettings);
}

BoolCheckbox::BoolCheckbox(QString name, bool value, QWidget* parent)
    : QWidget{parent}, m_layout{this}
{
    m_varLabel = new QLabel(name, this);
    m_radioButton = new QRadioButton(this);
    m_radioButton->setChecked(value);
    connect(m_radioButton, &QRadioButton::toggled, this,
            &BoolCheckbox::valueChanged);

    m_layout.addWidget(m_varLabel);
    m_layout.addWidget(m_radioButton);
}

SliderWidget::SliderWidget(QWidget* parent) : QWidget(parent), m_layout{this}
{
    m_varLabel = new QLabel(this);
    m_slider = new QSlider(Qt::Horizontal, this);
    m_valLabel = new QLabel(this);

    m_slider->setMaximum(m_sliderMaximum);
    m_slider->setMinimum(m_sliderMinimum);
    m_slider->setTickInterval(m_tickInterval);

    m_layout.addWidget(m_varLabel);
    m_layout.addWidget(m_slider);
    m_layout.addWidget(m_valLabel);
}

void SliderWidget::setSliderBounds(int min, int max)
{
    m_sliderMaximum = max;
    m_sliderMinimum = min;
    m_slider->setMaximum(m_sliderMaximum);
    m_slider->setMinimum(m_sliderMinimum);
};

IntSlider::IntSlider(QString name, int value, QWidget* parent)
    : SliderWidget{parent}
{
    m_varLabel->setText(name);
    auto updateLabel = [this](int value) {
        QString s;
        s.setNum(value);
        m_valLabel->setText(s);
    };

    connect(m_slider, &QSlider::valueChanged, updateLabel);
    connect(m_slider, &QSlider::valueChanged, this, &IntSlider::valueChanged);

    setValue(value);
}

FloatSlider::FloatSlider(QString name, float value, QWidget* parent)
    : SliderWidget{parent}
{

    m_varLabel->setText(name);

    auto updateLabel = [this](int value) {
        QString s;
        s.setNum(intToFloat(value), 'f', 2);
        m_valLabel->setText(s);
    };

    connect(m_slider, &QSlider::valueChanged, updateLabel);
    connect(m_slider, &QSlider::valueChanged, this, &FloatSlider::valueChanged);

    setValue(value);
}

float FloatSlider::getValue() { return intToFloat(m_slider->value()); }

float FloatSlider::intToFloat(int value)
{
    float t = static_cast<float>((value - m_sliderMinimum)) /
              static_cast<float>((m_sliderMaximum - m_sliderMinimum));
    float newValue = (1 - t) * m_lowerBound + t * m_upperBound;
    return newValue;
}

int FloatSlider::floatToInt(float value)
{
    float t = (value - m_lowerBound) / (m_upperBound - m_lowerBound);
    int newValue =
        static_cast<int>((1 - t) * m_sliderMinimum + t * m_sliderMaximum);
    return newValue;
}

void FloatSlider::setBounds(float min, float max)
{
    m_lowerBound = min;
    m_upperBound = max;
    setValue(getValue());
};

void FloatSlider::setValue(float value)
{
    m_slider->setValue(floatToInt(value));
}