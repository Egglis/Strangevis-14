#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "renderwidget.h"
#include "environment.h"

#include <QMainWindow>
#include <QVector>

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
    void createWidget();
    QVector<RenderWidget*> m_renderWidgets;
    QOpenGLWidget* m_hiddenWidget;
    QWidget* m_mainWidget;
    RectangularGridLayout* m_layout;
    Environment* m_environment;
};
#endif // MAINWINDOW_H
