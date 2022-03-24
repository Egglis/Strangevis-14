#ifndef MAINWINDOWWIDGET_H
#define MAINWINDOWWIDGET_H

#include "renderwidget.h"
#include "parameterwidget.h"
#include "transferfunctionwidget.h"

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;

class MainWindowWidget : public QWidget {
    Q_OBJECT

  public:
    MainWindowWidget(QWidget* p_3dRenderWidget, QWidget* p_3dToolBarWidget, QWidget* p_2dRenderWidget, QWidget* p_2dToolBarWidget, QWidget* parent);

  private:
    QVBoxLayout* create3dRenderLayout();
    QVBoxLayout* create2dRenderLayout();

    QWidget* m_3dRenderWidget;
    QWidget* m_3dToolBarWidget;
    QWidget* m_2dRenderWidget;
    QWidget* m_2dToolBarWidget;

    QVBoxLayout* m_3dRenderLayout;
    QVBoxLayout* m_2dRenderLayout;

    QHBoxLayout* m_mainWindowLayout;

};
#endif // MAINWINDOWWIDGET_H
