#ifndef RENDERSETTINGSPROPERTIES_H
#define RENDERSETTINGSPROPERTIES_H

#include <QObject>
#include <QString>

using RenderTypes = std::variant<float, int, bool>;
using RenderSettings = std::map<QString, RenderTypes>;

class RenderSettingsProperties : public QObject
{
    Q_OBJECT
  public:
    RenderSettingsProperties(RenderSettings renderSettings);
    const RenderSettings& renderSettings() const { return m_renderSettings; };
  public slots:
    void updateRenderSettings(RenderSettings renderSettings);
    void updateSingleRenderSetting(QString, RenderTypes);

  signals:
    void renderSettingsChanged(RenderSettings renderSettings);

  private:
    RenderSettings m_renderSettings;
};

#endif // RENDERSETTINGSPROPERTIES_H
