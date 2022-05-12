#ifndef LIGHTCONTROLSETTINGSWIDGET_H
#define LIGHTCONTROLSETTINGSWIDGET_H

#include "rendersettingswidget.h"

namespace Settings
{

static QList<QString>
    LIGHT_SETTINGS_ORDER({"headLight", "ambientInt", "diffuseInt", "specOff",
                    "specInt", "specCoeff"});
};

class LightControlSettingsWidget : public QWidget
{
    Q_OBJECT
  public:
    LightControlSettingsWidget(const std::shared_ptr<ISharedProperties> properties,
                         QWidget* parent = nullptr);
  public slots:
    void updateRenderSettings();

  private:
    void setupWidgets();
    void setupSettings();
    const std::shared_ptr<ISharedProperties> m_properties;

    QVBoxLayout* m_layout;
    RenderSettings m_renderSettings;
    QMap<QString, FloatSlider*> m_floatSliders;
    QMap<QString, BoolCheckbox*> m_boolCheckboxes;
    QMap<QString, IntSlider*> m_intSliders;
};
#endif // LIGHTCONTROLSETTINGSWIDGET_H
