#include "application.h"
#include "mainwindow.h"

#include <QSurfaceFormat>

int main(int argc, char* argv[])
{

    StrangevisVisualizerApplication app{argc, argv};

    auto properties = app.properties();
    auto colorMapStore = app.colorMapStore();

    MainWindow window{properties, colorMapStore};
    window.resize(768, 768);
    window.show();

    return app.exec();
}
