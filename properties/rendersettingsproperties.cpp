#include "rendersettingsproperties.h"

RenderSettingsProperties::RenderSettingsProperties(
    RenderSettings renderSettings)
    : m_renderSettings{renderSettings}
{
}

void RenderSettingsProperties::updateRenderSettings(
    RenderSettings renderSettings)
{
    m_renderSettings = renderSettings;
    emit renderSettingsChanged(m_renderSettings);
}

void RenderSettingsProperties::updateSingleRenderSetting(QString key, RenderTypes value)
{
    m_renderSettings[key] = value;
    emit renderSettingsChanged(m_renderSettings);
}
