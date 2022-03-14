#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui/renderwidget.h"
#include "ui/parameterwidget.h"
#include "environment.h"

#include <QMainWindow>
#include <QVector>
#include <memory>

class RectangularGridLayout;
class MainWindowWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(Environment* env, QWidget* parent = nullptr);

  public slots:
    void fileOpen();

  private:
    std::shared_ptr<SharedProperties> x_properties;
    MainWindowWidget* m_mainWidget;
    Environment* m_environment;
};
#endif // MAINWINDOW_H
