#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/renderwidget.h"
#include "ui/parameterwidget.h"
#include "environment.h"

#include <QMainWindow>
#include <QVector>
#include <memory>

class RectangularGridLayout;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(Environment* env, QWidget* parent = nullptr);

  public slots:
    void fileOpen();
    void addWidget();
    void removeWidget();

  private:
    std::shared_ptr<SharedProperties> x_properties;
    QVector<RenderWidget*> m_renderWidgets;
    QOpenGLWidget* m_hiddenWidget;
    QWidget* m_mainWidget;
    ParameterWidget* m_parameterWidget;
    RectangularGridLayout* m_renderLayout;
    QVBoxLayout* m_mainApplicationLayout;
    Environment* m_environment;
};
#endif // MAINWINDOW_H
