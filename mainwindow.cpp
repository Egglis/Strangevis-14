#include "mainwindow.h"

#include "properties/sharedproperties.h"
#include "ui/mainwindowwidget.h"
#include "ui/obliqueslicewidget.h"
#include "ui/parameterwidget.h"
#include "ui/rectangulargridlayout.h"
#include "ui/renderwidget.h"
#include "ui/transferfunctionwidget.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(std::shared_ptr<ITextureStore> textureStore,
                       std::shared_ptr<ISharedProperties> properties,
                       std::shared_ptr<tfn::IColorMapStore> colorMapStore,
                       QWidget* parent)
    : QMainWindow(parent), m_textureStore{textureStore}, m_properties{
                                                             properties}, m_colorMapStore{colorMapStore}
{

    QMenu* fileMenu = new QMenu("File");

    QAction* fileOpenAction = new QAction("Open", this);
    connect(fileOpenAction, &QAction::triggered, this, &MainWindow::fileOpen);
    fileMenu->addAction(fileOpenAction);

    menuBar()->addMenu(fileMenu);

    RenderWidget* p_3dRenderWidget =
        new RenderWidget(m_textureStore, m_properties, this);
    ExtendedParameterWidget* p_3dToolBarWidget =
        new ExtendedParameterWidget(m_properties, m_colorMapStore, this);
    ObliqueSliceWidget* p_2dRenderWidget =
        new ObliqueSliceWidget(m_textureStore, m_properties, this);
    ObliqueSliceRotationWidget* p_2dToolBarWidget =
        new ObliqueSliceRotationWidget(m_properties, this);

    connect(p_2dToolBarWidget, &ObliqueSliceRotationWidget::flipHorizontal,
            p_2dRenderWidget->renderWidget(),
            &ObliqueSliceRenderWidget::flipHorizontal);
    connect(p_2dToolBarWidget, &ObliqueSliceRotationWidget::flipVertical,
            p_2dRenderWidget->renderWidget(),
            &ObliqueSliceRenderWidget::flipVertical);

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
        m_textureStore->volume().load(fileName);
    }
}
