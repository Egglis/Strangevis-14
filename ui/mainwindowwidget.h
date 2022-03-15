#ifndef MAINWINDOWWIDGET_H
#define MAINWINDOWWIDGET_H

#include "renderwidget.h"
#include "parameterwidget.h"

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;

class MainWindowWidget : public QWidget {
    Q_OBJECT

  public:
    MainWindowWidget(RenderWidget* p_3dRenderWidget, ParameterWidget* p_3dToolBarWidget, RenderWidget* p_2dRenderWidget, ParameterWidget* p_2dToolBarWidget, QWidget* parent);

  private:
    QVBoxLayout* create3dRenderLayout();
    QVBoxLayout* create2dRenderLayout();
    RenderWidget* m_3dRenderWidget;
    ParameterWidget* m_3dToolBarWidget;
    RenderWidget* m_2dRenderWidget;
    ParameterWidget* m_2dToolBarWidget;
    QHBoxLayout* m_mainWindowLayout;
    QVBoxLayout* m_3dRenderLayout;
    QVBoxLayout* m_2dRenderLayout;
};
#endif // MAINWINDOWWIDGET_H