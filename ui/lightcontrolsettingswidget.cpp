
#include "lightcontrolsettingswidget.h"

LightControlSettingsWidget::LightControlSettingsWidget(
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

void LightControlSettingsWidget::setupSettings()
{
    m_floatSliders.insert("ambientInt",
                          new FloatSlider("Ambient Intensity:", 0.1f));
    m_floatSliders["ambientInt"]->hide();
    m_floatSliders.insert("diffuseInt", new FloatSlider("Brightness:", 1.5f));
    m_floatSliders.insert("specInt",
                          new FloatSlider("Specular Intensity:", 1.0f));
    m_floatSliders["specInt"]->hide();

    FloatSlider* specCoeff = new FloatSlider("Specular Coefficient:", 60.0f);
    specCoeff->setBounds(0.0f, 100.0f);
    specCoeff->setValue(60.0f);
    m_floatSliders.insert("specCoeff", specCoeff);
    m_floatSliders["specCoeff"]->hide();

    m_boolCheckboxes.insert("specOff",
                            new BoolCheckbox("Specular Highlights:", true));
    m_boolCheckboxes["specOff"]->hide();

    m_boolCheckboxes.insert("headLight",
                            new BoolCheckbox("Use Head Light:", false));

    connect(m_boolCheckboxes["specOff"], &BoolCheckbox::valueChanged,
            [this](bool vis) {
                m_floatSliders["specInt"]->setEnabled(vis);
                m_floatSliders["specCoeff"]->setEnabled(vis);
            });

    connect(&m_properties->renderSettings(),
            &RenderSettingsProperties::renderSettingsChanged,
            [this](auto settings) {
                if (settings.contains("maxInt"))
                {
                    m_boolCheckboxes["headLight"]->setEnabled(
                        !std::get<bool>(settings["maxInt"]));
                    m_floatSliders["diffuseInt"]->setEnabled(
                        !std::get<bool>(settings["maxInt"]));
                }
            });
};

void LightControlSettingsWidget::setupWidgets()
{
    QList<QWidget*> order = QList<QWidget*>(
        Settings::RENDER_SETTINGS_ORDER.length(), new QWidget());
    for (auto [key, w] : m_boolCheckboxes.toStdMap())
    {
        connect(w, &BoolCheckbox::valueChanged, [this, key, w]() {
            m_properties->renderSettings().updateSingleRenderSetting(
                key, w->getValue());
        });
        order.replace(Settings::LIGHT_SETTINGS_ORDER.indexOf(key),
                      static_cast<QWidget*>(w));
    }
    for (auto [key, w] : m_floatSliders.toStdMap())
    {
        connect(w, &SliderWidget::valueChanged, [this, key, w]() {
            m_properties->renderSettings().updateSingleRenderSetting(
                key, w->getValue());
        });
        order.replace(Settings::LIGHT_SETTINGS_ORDER.indexOf(key),
                      static_cast<QWidget*>(w));
    }
    for (auto [key, w] : m_intSliders.toStdMap())
    {
        connect(w, &SliderWidget::valueChanged, [this, key, w]() {
            m_properties->renderSettings().updateSingleRenderSetting(
                key, w->getValue());
        });
        order.replace(Settings::LIGHT_SETTINGS_ORDER.indexOf(key),
                      static_cast<QWidget*>(w));
    }

    for (auto w : order)
    {
        m_layout->addWidget(w);
    }
}

void LightControlSettingsWidget::updateRenderSettings()
{
    for (auto s : m_floatSliders.keys())
    {
        m_properties->renderSettings().updateSingleRenderSetting(
            s, m_floatSliders[s]->getValue());
    }
    for (auto s : m_boolCheckboxes.keys())
    {
        m_properties->renderSettings().updateSingleRenderSetting(
            s, m_boolCheckboxes[s]->getValue());
    }
    for (auto s : m_intSliders.keys())
    {
        m_properties->renderSettings().updateSingleRenderSetting(
            s, m_intSliders[s]->getValue());
    }
}
