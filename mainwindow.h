#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "texturestore.h"
#include "ui/renderwidget.h"

#include <QMainWindow>
#include <QVector>
#include <memory>

class RectangularGridLayout;
class MainWindowWidget;
class HistogramWidget;
class RenderSettingsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(std::shared_ptr<ISharedProperties> properties,
               std::shared_ptr<tfn::IColorMapStore> colorMapStore,
               QWidget* parent = nullptr);

  public slots:
    void fileOpen();
  private slots:
    void openHistogram();

  private:
    void createHistogramWidget();
    void createRenderSettingsWidget();
    const std::shared_ptr<ISharedProperties> m_properties;
    std::unique_ptr<ITextureStore> m_textureStore;
    const std::shared_ptr<tfn::IColorMapStore> m_colorMapStore;
    MainWindowWidget* m_mainWidget;
    HistogramWidget* m_histogramWidget;
    RenderSettingsWidget* m_renderSettingsWidget;

};
#endif // MAINWINDOW_H
