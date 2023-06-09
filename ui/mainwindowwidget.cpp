#include "mainwindowwidget.h"

MainWindowWidget::MainWindowWidget(QWidget* p_3dRenderWidget,
                                   QWidget* p_3dToolBarWidget,
                                   QWidget* p_2dRenderWidget,
                                   QWidget* p_2dToolBarWidget, QWidget* parent)
    : QWidget(parent), m_3dRenderWidget{p_3dRenderWidget},
      m_3dToolBarWidget{p_3dToolBarWidget}, m_2dRenderWidget{p_2dRenderWidget},
      m_2dToolBarWidget{p_2dToolBarWidget}
{
    m_mainWindowLayout = new QHBoxLayout(this);
    m_3dRenderLayout = create3dRenderLayout();
    m_2dRenderLayout = create2dRenderLayout();

    m_2dRenderWidget->setParent(this);
    m_2dToolBarWidget->setParent(this);
    m_3dRenderWidget->setParent(this);
    m_3dToolBarWidget->setParent(this);

    m_mainWindowLayout->addLayout(m_3dRenderLayout, 3);
    m_mainWindowLayout->addLayout(m_2dRenderLayout, 2);
}

QVBoxLayout* MainWindowWidget::create2dRenderLayout()
{
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_2dRenderWidget, 5);
    vLayout->addWidget(m_2dToolBarWidget, 1);
    return vLayout;
}

QVBoxLayout* MainWindowWidget::create3dRenderLayout()
{
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(m_3dRenderWidget, 4);
    vLayout->addWidget(m_3dToolBarWidget, 1);
    return vLayout;
}

void MainWindowWidget::toggleFileLoadingInProgressOverlay(bool state)
{
    if (state)
    {
        m_progressBar = new QProgressBar(this);
        m_progressBar->setGeometry(16, 16, 320, 16);
        m_progressBar->setMaximum(0);
        m_progressBar->setMinimum(0);
        m_progressBar->raise();
        m_progressBar->show();
    }
    else
        delete m_progressBar;
}
