#include "mainwindow.h"

#include "properties/sharedproperties.h"
#include "ui/mainwindowwidget.h"
#include "ui/obliquesliceinteractor.h"
#include "ui/parameterwidget.h"
#include "ui/rectangulargridlayout.h"
#include "ui/renderwidget.h"
#include "ui/transferfunctionwidget.h"
#include "ui/histogramwidget.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>

MainWindow::MainWindow(std::shared_ptr<ITextureStore> textureStore,
                       std::shared_ptr<ISharedProperties> properties,
                       std::shared_ptr<tfn::IColorMapStore> colorMapStore,
                       QWidget* parent)
    : QMainWindow(parent), m_textureStore{textureStore},
      m_properties{properties}, m_colorMapStore{colorMapStore}
{

    QMenu* fileMenu = new QMenu("File");

    QAction* fileOpenAction = new QAction("Open", this);
    connect(fileOpenAction, &QAction::triggered, this, &MainWindow::fileOpen);
    fileMenu->addAction(fileOpenAction);


    createHistogramWidget();
    QAction* openHistogramAction = new QAction("Open Histogram", this);
    connect(openHistogramAction, &QAction::triggered, this, &MainWindow::openHistogram);
    fileMenu->addAction(openHistogramAction);

    menuBar()->addMenu(fileMenu);

    RayCastingInteractor* p_3dRenderWidget =
        new RayCastingInteractor(m_textureStore, m_properties, this);
    ExtendedParameterWidget* p_3dToolBarWidget =
        new ExtendedParameterWidget(m_properties, m_colorMapStore, this);
    ObliqueSliceInteractor* p_2dRenderWidget =
        new ObliqueSliceInteractor(m_textureStore, m_properties, this);
    ObliqueSliceRotationWidget* p_2dToolBarWidget =
        new ObliqueSliceRotationWidget(m_properties, this);

    connect(p_2dToolBarWidget, &ObliqueSliceRotationWidget::flipHorizontal,
            p_2dRenderWidget, &ObliqueSliceRenderWidget::flipHorizontal);
    connect(p_2dToolBarWidget, &ObliqueSliceRotationWidget::flipVertical,
            p_2dRenderWidget, &ObliqueSliceRenderWidget::flipVertical);

    m_mainWidget =
        new MainWindowWidget(p_3dRenderWidget, p_3dToolBarWidget,
                             p_2dRenderWidget, p_2dToolBarWidget, this);
    connect(&textureStore->volume(), &Volume::loadingStartedOrStopped,
            m_mainWidget,
            &MainWindowWidget::toggleFileLoadingInProgressOverlay);
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

void MainWindow::createHistogramWidget()
{
    m_histogramWidget = new HistogramWidget();
    connect(&m_textureStore->volume(), &Volume::histogramCalculated, m_histogramWidget, &HistogramWidget::histogramChanged);
    m_histogramWidget->setAttribute(Qt::WA_QuitOnClose, false);
}

void MainWindow::openHistogram()
{
    m_histogramWidget->show();
}
