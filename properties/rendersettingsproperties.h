#ifndef RENDERSETTINGSPROPERTIES_H
#define RENDERSETTINGSPROPERTIES_H

#include <QObject>
#include <QString>
#include <any>

enum class SettingTypes { FLOAT, INT, BOOL };

using RenderSettings = std::map<QString, std::pair<SettingTypes, std::any>>;
class RenderSettingsProperties : public QObject
{
    Q_OBJECT
  public:
    RenderSettingsProperties(RenderSettings renderSettings);
    const RenderSettings& renderSettings() const { return m_renderSettings; };
  public slots:
    void updateRenderSettings(RenderSettings renderSettings);

  signals:
    void renderSettingsChanged(RenderSettings renderSettings);

  private:
    RenderSettings m_renderSettings;
};

#endif // RENDERSETTINGSPROPERTIES_H
