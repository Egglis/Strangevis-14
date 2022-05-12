#include "rendersettingswidget.h"

RenderSettingsWidget::RenderSettingsWidget(
    const std::shared_ptr<ISharedProperties> properties, QWidget* parent)
    : m_properties{properties}, QWidget(parent)
{
    m_renderSettings = {};
    m_layout = new QVBoxLayout();
    setupSettings();
    setupWidgets();
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(QMargins{0, 0, 0, 0});
    updateRenderSettings();

    this->setLayout(m_layout);
}

void RenderSettingsWidget::setupSettings()
{
    m_boolCheckboxes.insert(
        "maxInt", new BoolCheckbox("Maximum intensity projection:", false));

    m_boolCheckboxes.insert("sliceModel",
                            new BoolCheckbox("Slice model:", false));
    m_boolCheckboxes.insert("sliceSide",
                            new BoolCheckbox("Swap slicing side:", false));

    m_boolCheckboxes.insert("showSlice",
                            new BoolCheckbox("Show slice:", true));
    m_boolCheckboxes.insert(
        "defaultSliceNr", new BoolCheckbox("Default quality:", true));

    IntSlider* sliceNr = new IntSlider("Quality:", 257);
    sliceNr->setSliderBounds(1, 1000);
    sliceNr->setValue(257);
    sliceNr->setEnabled(!m_boolCheckboxes["defaultSliceNr"]->getValue());
    m_intSliders.insert("sliceNr", sliceNr);

    connect(m_boolCheckboxes["defaultSliceNr"], &BoolCheckbox::valueChanged,
            [this](bool vis) { m_intSliders["sliceNr"]->setEnabled(!vis); });

    connect(m_boolCheckboxes["sliceModel"], &BoolCheckbox::valueChanged,
            [this](const bool vis) {
                m_boolCheckboxes["sliceSide"]->setEnabled(vis);
            });
};

void RenderSettingsWidget::setupWidgets()
{
    QList<QWidget*> order = QList<QWidget*>(
        Settings::RENDER_SETTINGS_ORDER.length(), new QWidget());
    for (auto [key, w] : m_boolCheckboxes.toStdMap())
    {
        connect(w, &BoolCheckbox::valueChanged, [this, key, w]() {
            m_properties->renderSettings().updateSingleRenderSetting(
                key, w->getValue());
        });
        order.replace(Settings::RENDER_SETTINGS_ORDER.indexOf(key),
                      static_cast<QWidget*>(w));
    }
    for (auto [key, w] : m_floatSliders.toStdMap())
    {
        connect(w, &SliderWidget::valueChanged, [this, key, w] (){
            m_properties->renderSettings().updateSingleRenderSetting(
                key, w->getValue());
        });
        order.replace(Settings::RENDER_SETTINGS_ORDER.indexOf(key),
                      static_cast<QWidget*>(w));
    }
    for (auto [key, w] : m_intSliders.toStdMap())
    {
        connect(w, &SliderWidget::valueChanged, [this, key, w] (){
            m_properties->renderSettings().updateSingleRenderSetting(
                key, w->getValue());
        });
        order.replace(Settings::RENDER_SETTINGS_ORDER.indexOf(key),
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
        m_properties->renderSettings().updateSingleRenderSetting(s, m_floatSliders[s]->getValue());
    }
    for (auto s : m_boolCheckboxes.keys())
    {
        m_properties->renderSettings().updateSingleRenderSetting(s, m_boolCheckboxes[s]->getValue());
    }
    for (auto s : m_intSliders.keys())
    {
        m_properties->renderSettings().updateSingleRenderSetting(s, m_intSliders[s]->getValue());
    }
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
