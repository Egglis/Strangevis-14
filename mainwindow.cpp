#include "mainwindow.h"

#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(Environment* env, QWidget* parent)
    : QMainWindow(parent), m_environment(env)
{

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
    m_layout = new QHBoxLayout(m_mainWidget);
    setCentralWidget(m_mainWidget);
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

void MainWindow::createWidget(bool create)
{
    RenderWidget* widget = new RenderWidget(m_environment, m_mainWidget);
    m_layout->addWidget(widget);
    m_renderWidgets.push_back(widget);
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
    createWidget(createNewWidget);
}

void MainWindow::removeWidget()
{
    if (m_renderWidgets.empty())
    {
        RenderWidget* widget = m_renderWidgets.back();
        m_renderWidgets.pop_back();

        if (m_renderWidgets.empty())
        {
            widget->hide();
        } else
        {
            delete widget;
        }
    }
}
