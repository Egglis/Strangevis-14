#include "renderwidget.h"

#include "../geometry.h"
#include "../transfertexture.h"
#include "parameterwidget.h"
#include "transferwidget/transferfunctionwidget.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>

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

void ExtendedParameterWidget::histogramChanged(std::vector<float> normalizedHistogramData){
    m_transferWidget.getGraph()->setHistogramData(normalizedHistogramData);
}

RayCastingInteractor::RayCastingInteractor(
    std::unique_ptr<ITextureStore>& textureStore,
    std::shared_ptr<ISharedProperties> properties, QWidget* parent,
    Qt::WindowFlags f)
    : RayCastingWidget{RenderProperties{
                           1.0,
                           QVector3D{0, 0, -2.0f * static_cast<float>(sqrt(3))},
                           properties->transferFunction().colorMap(),
                           properties->clippingPlane().plane(),
                           properties->gradientMethod().method()},
                       textureStore, properties, parent, f},
      m_properties{properties}
{
    connect(&m_properties.get()->clippingPlane(),
            &ClippingPlaneProperties::clippingPlaneChanged, this,
            [this](const Plane& plane) { updateClippingPlane(plane); });

    connect(&m_properties.get()->gradientMethod(),
            &GradientProperties::gradientMethodChanged, this,
            &RayCastingInteractor::changeGradientMethod);

    connect(&m_properties.get()->transferFunction(),
            &tfn::TransferProperties::transferFunctionChanged, this,
            [this]() { update(); });

    connect(&m_properties.get()->transferFunction(),
            &tfn::TransferProperties::colorMapChanged, this,
            &RayCastingInteractor::changeTransferFunction);

    m_refreshTimer = nullptr;
    setMouseTracking(true);
    setUpdateAfterMouseEventStops(true);
}

void RayCastingInteractor::setUpdateAfterMouseEventStops(bool update)
{
    if (m_refreshTimer != nullptr)
        delete m_refreshTimer;
    m_refreshTimer = new QTimer{};
    m_refreshTimer->setInterval(10);
    m_refreshTimer->setSingleShot(true);
    if (update)
        m_refreshTimer->callOnTimeout([this]() { this->update(); });
    else
        m_refreshTimer->callOnTimeout([this]() {});
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
                rotateCamera();
            }
        }
        m_previousPosition = m_currentPosition;
    }
    else
    {
        update();
    }
    if (m_refreshTimer != nullptr)
        m_refreshTimer->start();
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
