#include "renderwidget.h"

#include "../geometry.h"
#include "../transfertexture.h"
#include "transferwidget/transferfunctionwidget.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>

StackedRayCastingWidget::StackedRayCastingWidget(
    std::unique_ptr<ITextureStore>& textureStore,
    std::shared_ptr<ISharedProperties> properties, QWidget* renderSettings, QWidget* lightSettings, QWidget* parent,
    Qt::WindowFlags f)
    : QWidget{parent}, m_camera{}
{
    m_rayCastingWidget =
        new RayCastingInteractor{textureStore, properties, m_camera, this, f};
    m_imguizmoWidget = new ImguizmoWidget{properties, m_camera, renderSettings, lightSettings, this, f};
    m_imguizmoWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_imguizmoWidget->setAttribute(Qt::WA_AlwaysStackOnTop);
    m_layout = new QStackedLayout(this);
    m_layout->setStackingMode(QStackedLayout::StackAll);
    m_layout->addWidget(m_imguizmoWidget);
    m_layout->addWidget(m_rayCastingWidget);
    setMouseTracking(true);

    connect(m_imguizmoWidget, &ImguizmoWidget::updateScene,
            [this]() { m_rayCastingWidget->update(); });
}
void StackedRayCastingWidget::mousePressEvent(QMouseEvent* p_event)
{
    m_rayCastingWidget->mousePressEvent(p_event);
    m_imguizmoWidget->update();
}

void StackedRayCastingWidget::mouseMoveEvent(QMouseEvent* p_event)
{
    m_rayCastingWidget->mouseMoveEvent(p_event);
    m_imguizmoWidget->update();
}

void StackedRayCastingWidget::wheelEvent(QWheelEvent* p_event)
{
    m_rayCastingWidget->wheelEvent(p_event);
    m_imguizmoWidget->update();
}

ExtendedParameterWidget::ExtendedParameterWidget(
    const std::shared_ptr<ISharedProperties>& properties,
    const std::shared_ptr<const tfn::IColorMapStore> colorMapStore,
    QWidget* parent)
    : QWidget(parent),
      m_transferWidget(properties, colorMapStore, this), m_layout{this}
{
    m_layout.addWidget(&m_transferWidget);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
}

void ExtendedParameterWidget::histogramChanged(
    std::vector<float> normalizedHistogramData)
{
    m_transferWidget.getGraph()->setHistogramData(normalizedHistogramData);
}

RayCastingInteractor::RayCastingInteractor(
    std::unique_ptr<ITextureStore>& textureStore,
    std::shared_ptr<ISharedProperties> properties, CameraProperties& camera,
    QWidget* parent, Qt::WindowFlags f)
    : RayCastingWidget{RenderProperties{
                           1.0,
                           QVector3D{0, 0, -2.0f * static_cast<float>(sqrt(3))},
                           properties->transferFunction().colorMap(),
                           properties->clippingPlane().plane(),
                           properties->renderSettings().renderSettings()},
                       textureStore,
                       properties,
                       camera,
                       parent,
                       f},
      m_properties{properties}
{

    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged, this,
            [this](const Plane& plane) { updateClippingPlane(plane); });

    connect(&m_properties.get()->transferFunction(),
            &tfn::TransferProperties::transferFunctionChanged, this,
            [this]() { update(); });

    connect(&m_properties.get()->transferFunction(),
            &tfn::TransferProperties::colorMapChanged, this,
            &RayCastingInteractor::changeTransferFunction);

    connect(&m_properties.get()->renderSettings(),
            &RenderSettingsProperties::renderSettingsChanged, this,
            &RayCastingInteractor::changeRenderSettings);

}

void RayCastingInteractor::mousePressEvent(QMouseEvent* p_event)
{
    m_currentPosition = p_event->position();
    m_previousPosition = m_currentPosition;

    update();
}

void RayCastingInteractor::mouseMoveEvent(QMouseEvent* p_event)
{
    if (!ImGuizmo::IsOver())
    {
        m_currentPosition = p_event->position();
        if (p_event->buttons() & Qt::LeftButton)
        {
            if (m_currentPosition != m_previousPosition)
            {
                if (p_event->modifiers() == Qt::ShiftModifier)
                {
                    moveLightSource();
                }
                else
                {
                    rotateCamera();
                }
            }
        }
        m_previousPosition = m_currentPosition;
    }
}

// Scrolling wheel event
void RayCastingInteractor::wheelEvent(QWheelEvent* p_event)
{
    QPoint deg = p_event->angleDelta();
    float zoomScale = 1.0;

    if (deg.y() < 0)
    {
        zoomScale = zoomScale / 1.1;
    }
    if (deg.y() > 0)
    {
        zoomScale = zoomScale * 1.1;
    }
    zoomCamera(zoomScale);
}

void RayCastingInteractor::moveLightSource()
{
    std::visit(
        [this](const auto& arg) {
            if (!arg)
                RayCastingWidget::moveLightSource(arcballVector(
                    m_currentPosition.x(), m_currentPosition.y()));
        },
        m_properties.get()->renderSettings().renderSettings().at("headLight"));
}

void RayCastingInteractor::rotateCamera()
{
    QVector3D va =
        arcballVector(m_previousPosition.x(), m_previousPosition.y());
    QVector3D vb = arcballVector(m_currentPosition.x(), m_currentPosition.y());

    if (va != vb)
    {
        qreal angle =
            acos(qMax(-1.0f, qMin(1.0f, QVector3D::dotProduct(va, vb))));
        QVector3D axis = QVector3D::crossProduct(va, vb);

        RayCastingWidget::rotateCamera(angle, axis);
    }
}

QVector3D RayCastingInteractor::arcballVector(qreal x, qreal y)
{
    QVector3D p = QVector3D(
        2.0f * static_cast<float>(x) / static_cast<float>(this->width()) - 1.0f,
        -2.0f * static_cast<float>(y) / static_cast<float>(this->height()) +
            1.0f,
        0.0);

    float length2 = p.x() * p.x() + p.y() * p.y();

    if (length2 < 1.0f)
    {
        p.setZ(sqrtf(1.0f - length2));
    }
    else
    {
        p.normalize();
    }
    return p;
}
