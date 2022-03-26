#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/renderwidget.h"
#include "ui/parameterwidget.h"
#include "texturestore.h"

#include <QMainWindow>
#include <QVector>
#include <memory>

class RectangularGridLayout;
class MainWindowWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(std::shared_ptr<ITextureStore> textureStore, std::shared_ptr<ISharedProperties> properties, std::shared_ptr<tfn::IColorMapStore> colorMapStore, QWidget* parent = nullptr);

  public slots:
    void fileOpen();

  private:
    const std::shared_ptr<ISharedProperties> m_properties;
    const std::shared_ptr<ITextureStore> m_textureStore;
    const std::shared_ptr<tfn::IColorMapStore> m_colorMapStore;
    MainWindowWidget* m_mainWidget;
};
#endif // MAINWINDOW_H
