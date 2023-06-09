#include "mainwindow.h"

#include "properties/clippingplaneproperties.h"
#include "properties/sharedproperties.h"
#include "ui/histogramwidget.h"
#include "ui/mainwindowwidget.h"
#include "ui/obliquesliceinteractor.h"
#include "ui/rectangulargridlayout.h"
#include "ui/rendersettingswidget.h"
#include "ui/lightcontrolsettingswidget.h"
#include "ui/renderwidget.h"
#include "ui/transferwidget/transferfunctionwidget.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QSizePolicy>

MainWindow::MainWindow(std::shared_ptr<ISharedProperties> properties,
                       std::shared_ptr<tfn::IColorMapStore> colorMapStore,
                       QWidget* parent)
    : QMainWindow(parent), m_textureStore{std::make_unique<TextureStore>()},
      m_properties{properties}, m_colorMapStore{colorMapStore}
{
    connect(&m_properties->transferFunction(),
            &tfn::TransferProperties::colorMapChanged, this,
            [this](const QString& cmap) {
                m_textureStore->transferFunction().setColorMap(
                    m_colorMapStore->colorMap(cmap).colorMapData());
            });
    connect(&m_properties->transferFunction(),
            &tfn::TransferProperties::transferFunctionChanged, this,
            [this](const auto& tfn) {
                m_textureStore->transferFunction().setTransferFunction(tfn);
            });

    QMenu* fileMenu = new QMenu("File");

    QAction* fileOpenAction = new QAction("Open", this);
    connect(fileOpenAction, &QAction::triggered, this, &MainWindow::fileOpen);
    fileMenu->addAction(fileOpenAction);

    createHistogramWidget();

    QAction* openHistogramAction = new QAction("Open Histogram", this);
    connect(openHistogramAction, &QAction::triggered, this,
            &MainWindow::openHistogram);
    fileMenu->addAction(openHistogramAction);

    menuBar()->addMenu(fileMenu);

    createRenderSettingsWidget();

    auto* p_3dRenderWidget =
        new StackedRayCastingWidget(m_textureStore, m_properties, m_renderSettingsWidget, m_lightSettingsWidget, this);
    auto* p_3dToolBarWidget =
        new ExtendedParameterWidget(m_properties, m_colorMapStore, this);
    auto* p_2dRenderWidget =
        new ObliqueSliceInteractor(m_textureStore, m_properties, this);
    auto* p_2dToolBarWidget =
        new ObliqueSliceRotationWidget(this);

    connect(p_2dToolBarWidget, &ObliqueSliceRotationWidget::flipHorizontal,
            p_2dRenderWidget, &ObliqueSliceRenderWidget::flipHorizontal);
    connect(p_2dToolBarWidget, &ObliqueSliceRotationWidget::flipVertical,
            p_2dRenderWidget, &ObliqueSliceRenderWidget::flipVertical);
    connect(p_2dToolBarWidget, &ObliqueSliceRotationWidget::resetClippingPlane,
            &m_properties->clippingPlane(), &ClippingPlaneProperties::reset);

    connect(&m_textureStore->volume(), &Volume::histogramCalculated,
            p_3dToolBarWidget, &ExtendedParameterWidget::histogramChanged);

    m_mainWidget =
        new MainWindowWidget(p_3dRenderWidget, p_3dToolBarWidget,
                             p_2dRenderWidget, p_2dToolBarWidget, this);
    connect(&m_textureStore->volume(), &Volume::loadingStartedOrStopped,
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
    connect(&m_textureStore->volume(), &Volume::histogramCalculated,
            m_histogramWidget, &HistogramWidget::histogramChanged);
    m_histogramWidget->setAttribute(Qt::WA_QuitOnClose, false);
}

void MainWindow::createRenderSettingsWidget()
{
    m_renderSettingsWidget = new RenderSettingsWidget(m_properties);
    m_lightSettingsWidget = new LightControlSettingsWidget(m_properties);
}

void MainWindow::openHistogram() { m_histogramWidget->show(); }
