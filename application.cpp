#include "application.h"

StrangevisVisualizerApplication::StrangevisVisualizerApplication(int argc,
                                                                 char* argv[])
    : QApplication{argc, argv}
{
    m_properties = std::make_shared<SharedProperties>();
    m_textureStore = std::make_shared<TextureStore>();
    m_colorMapStore = std::make_shared<tfn::ColorMapStore>();

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setMajorVersion(4);
    format.setMinorVersion(5);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOption(QSurfaceFormat::DeprecatedFunctions, true);
    QSurfaceFormat::setDefaultFormat(format);

    connect(&m_properties->colorMap(),
            &tfn::TransferProperties::transferFunctionChanged,
            this,
            [this](const QString& cmap)
            {
                m_textureStore->transferFunction().setColorMap(m_colorMapStore->colorMap(cmap).colorMapData());
            });
}
