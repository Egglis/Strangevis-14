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
    emit renderSettingsChanged(renderSettings);
}
