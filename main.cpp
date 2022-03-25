#include "mainwindow.h"

#include "properties/sharedproperties.h"
#include "texturestore.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setMajorVersion(4);
    format.setMinorVersion(5);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setOption(QSurfaceFormat::DeprecatedFunctions, true);
    QSurfaceFormat::setDefaultFormat(format);

    auto textureStore = std::make_shared<TextureStore>();
    auto properties = std::make_shared<SharedProperties>();

    MainWindow window{std::move(textureStore), std::move(properties)};
    window.resize(768, 768);
    window.show();

    return app.exec();
}
