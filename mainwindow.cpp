#include "mainwindow.h"

#include "ui/rectangulargridlayout.h"
#include "properties/sharedproperties.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(Environment* env, QWidget* parent)
    : QMainWindow(parent), m_environment(env)
{
    x_properties = std::make_shared<SharedProperties>();

    env->setParent(
        this); // Endure destruction of env on destruction of MainWindow

    QMenu* fileMenu = new QMenu("File");

    QAction* fileOpenAction = new QAction("Open", this);
    connect(fileOpenAction, &QAction::triggered, this, &MainWindow::fileOpen);
    fileMenu->addAction(fileOpenAction);

    QAction* fileAddWidgetAction = new QAction("Add Widget", this);
    connect(fileAddWidgetAction, &QAction::triggered, this,
            &MainWindow::addWidget);
    fileMenu->addAction(fileAddWidgetAction);

    QAction* fileRemoveWidgetAction = new QAction("Remove Widget", this);
    connect(fileRemoveWidgetAction, &QAction::triggered, this,
            &MainWindow::removeWidget);
    fileMenu->addAction(fileRemoveWidgetAction);

    menuBar()->addMenu(fileMenu);

    m_mainWidget = new QWidget(this);
    m_mainApplicationLayout = new QVBoxLayout(m_mainWidget);
    m_parameterWidget = new ParameterWidget(x_properties, m_mainWidget);
    m_renderLayout = new RectangularGridLayout(m_mainWidget);
    m_mainApplicationLayout->addLayout(m_renderLayout);
    m_mainApplicationLayout->addWidget(m_parameterWidget);
    m_mainApplicationLayout->setAlignment(m_parameterWidget, Qt::AlignHCenter);
    m_mainWidget->setLayout(m_mainApplicationLayout);
    setCentralWidget(m_mainWidget);

    addWidget();
}

void MainWindow::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Volume File",
                                                    QString(), "*.dat");

    if (!fileName.isEmpty())
    {
        m_environment->volume()->load(fileName);
    }
}

void MainWindow::addWidget()
{
    bool createNewWidget = true;

    if (m_renderWidgets.size() == 1)
    {
        RenderWidget* widget = m_renderWidgets.back();

        if (!widget->isVisible())
        {
            widget->show();
            createNewWidget = false;
            // TODO: reset any state of widget, since it was not newly created
        }
    }
    if (createNewWidget)
    {
        RenderWidget* widget = new RenderWidget(m_environment, x_properties, m_mainWidget);
        m_renderLayout->addWidgetRectangular(widget);
        m_renderWidgets.push_back(widget);
    }
}

void MainWindow::removeWidget()
{
    if (!m_renderWidgets.empty())
    {
        RenderWidget* widget = m_renderWidgets.back();

        if (m_renderWidgets.size() == 1)
        {
            widget->hide();
        }
        else
        {
            m_renderLayout->removeWidgetRectangular(widget);
            delete widget;
            m_renderWidgets.pop_back();
        }
    }
}
