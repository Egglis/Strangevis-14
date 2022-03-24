#include "environment.h"
#include "mainwindow.h"

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

    MainWindow window{};
    window.resize(768, 768);
    window.show();

    return app.exec();
}
