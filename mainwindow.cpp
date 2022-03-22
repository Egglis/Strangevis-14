#include "mainwindow.h"

#include "properties/sharedproperties.h"
#include "ui/mainwindowwidget.h"
#include "ui/renderwidget.h"
#include "ui/parameterwidget.h"
#include "ui/obliqueslicewidget.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_environment = std::make_shared<Environment>();
    m_properties = std::make_shared<SharedProperties>();

    QMenu* fileMenu = new QMenu("File");

    QAction* fileOpenAction = new QAction("Open", this);
    connect(fileOpenAction, &QAction::triggered, this, &MainWindow::fileOpen);
    fileMenu->addAction(fileOpenAction);

    menuBar()->addMenu(fileMenu);

    RenderWidget* p_3dRenderWidget =
        new RenderWidget(m_environment, m_properties, this);
    ParameterWidget* p_3dToolBarWidget =
        new ParameterWidget(m_properties, this);
    ObliqueSliceWidget* p_2dRenderWidget =
        new ObliqueSliceWidget(m_environment, m_properties, this);
    ParameterWidget* p_2dToolBarWidget =
        new ParameterWidget(m_properties, this);

    m_mainWidget =
        new MainWindowWidget(p_3dRenderWidget, p_3dToolBarWidget,
                             p_2dRenderWidget, p_2dToolBarWidget, this);
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
