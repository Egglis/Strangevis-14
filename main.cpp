#include "mainwindow.h"
#include "application.h"

#include <QSurfaceFormat>

int main(int argc, char* argv[])
{

    StrangevisVisualizerApplication app{argc, argv};

    auto textureStore = app.textureStore();
    auto properties = app.properties();
    auto colorMapStore = app.colorMapStore();

    MainWindow window{textureStore, properties, colorMapStore};
    window.resize(768, 768);
    window.show();

    return app.exec();
}
