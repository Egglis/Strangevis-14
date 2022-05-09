#include "rendersettingswidget.h"

RenderSettingsWidget::RenderSettingsWidget(
    const std::shared_ptr<ISharedProperties> properties, QWidget* parent)
    : m_properties{properties}, QWidget(parent)
{
    m_renderSettings = {};
    m_layout = new QVBoxLayout();

    m_floatSliders.insert("ambientInt", new FloatSlider("Ambient Intensity", 0.1f));
    m_floatSliders.insert("diffuseInt", new FloatSlider("Diffuse Intensity", 1.5f));
    m_floatSliders.insert("specInt", new FloatSlider("Specular Intensity", 0.0f));
    m_floatSliders.insert("specCoeff", new FloatSlider("Specular Coefficient", 80.0f));

    m_floatSliders["specCoeff"]->setBounds(0.0f, 100.0f);

    m_boolCheckboxes.insert("specOff", new BoolCheckbox("Specular Highlights", false));

    setupWidgets();
    updateRenderSettings();

    this->setLayout(m_layout);
}


void RenderSettingsWidget::setupWidgets(){
    for(auto w : m_boolCheckboxes.values()){
        connect(w, &BoolCheckbox::valueChanged, this, &RenderSettingsWidget::updateRenderSettings);
        m_layout->addWidget(w);
    }
    for(auto w : m_floatSliders.values()){
        connect(w, &SliderWidget::valueChanged, this, &RenderSettingsWidget::updateRenderSettings);
        m_layout->addWidget(w);
    }
}

void RenderSettingsWidget::updateRenderSettings(){
    for(auto s : m_floatSliders.keys()){
        m_renderSettings[s] = std::make_pair(SettingTypes::FLOAT, static_cast<std::any>(m_floatSliders[s]->getValue()));
    }
    for(auto s : m_boolCheckboxes.keys()) {
        m_renderSettings[s] = std::make_pair(SettingTypes::BOOL, static_cast<std::any>(m_boolCheckboxes[s]->getValue()));
    }
    m_properties->renderSettings().updateRenderSettings(m_renderSettings);
}

BoolCheckbox::BoolCheckbox(QString name, bool value, QWidget* parent)
: QWidget{parent}, m_layout{this}
{
    m_varLabel = new QLabel(name, this);
    m_radioButton = new QRadioButton(this);
    m_radioButton->setChecked(value);
    connect(m_radioButton, &QRadioButton::toggled, this, &BoolCheckbox::valueChanged);

    m_layout.addWidget(m_varLabel);
    m_layout.addWidget(m_radioButton);
}

SliderWidget::SliderWidget(QWidget* parent) : QWidget(parent), m_layout{this} {
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

void SliderWidget::setSliderBounds(int min, int max){
    m_sliderMaximum = max;
    m_sliderMinimum = min;
    m_slider->setMaximum(m_sliderMaximum);
    m_slider->setMinimum(m_sliderMinimum);
};

FloatSlider::FloatSlider(QString name, float value, QWidget* parent) : SliderWidget{parent}
{

    m_varLabel->setText(name);

    auto updateLabel = [this](int value) {
        QString s;
        s.setNum(intToFloat(value), 'f', 2);
        m_valLabel->setText(s);
    };

    connect(m_slider, &QSlider::valueChanged, updateLabel);
    connect(m_slider, &QSlider::valueChanged, this,
            &FloatSlider::valueChanged);

    setValue(value);
}

float FloatSlider::getValue() {
    return intToFloat(m_slider->value());
}

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

void FloatSlider::setBounds(float min, float max) {
    m_lowerBound = min;
    m_upperBound = max;
};

void FloatSlider::setValue(float value)
{
    m_slider->setValue(floatToInt(value));
}