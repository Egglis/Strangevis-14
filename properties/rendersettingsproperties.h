#ifndef RENDERSETTINGSPROPERTIES_H
#define RENDERSETTINGSPROPERTIES_H

#include <QObject>
#include <QString>


using RenderSettings = std::map<QString, std::variant<float, int, bool>>;

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
