#include "renderwidget.h"

#include "../geometry.h"
#include "../transfertexture.h"
#include "parameterwidget.h"
#include "transferfunctionwidget.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtMath>

ExtendedParameterWidget::ExtendedParameterWidget(
    const std::shared_ptr<ISharedProperties>& properties,
    const std::shared_ptr<const tfn::IColorMapStore> colorMapStore,
    QWidget* parent)
    : QWidget(parent), m_parameterWidget(properties, this),
      m_transferWidget(properties, colorMapStore, this), m_layout{this}
{
    m_layout.addWidget(&m_transferWidget);
    m_layout.addWidget(&m_parameterWidget);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
}

RayCastingInteractor::RayCastingInteractor(
    const std::shared_ptr<ITextureStore> textureStore,
    const std::shared_ptr<ISharedProperties> properties, QWidget* parent,
    Qt::WindowFlags f)
    : RayCastingWidget(
          RenderProperties{1.0, properties->transferFunction().colorMap(),
                           properties->clippingPlane().plane(),
                           properties->gradientMethod().method(),
                           Projection::Orthographic},
          textureStore, parent, f),
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
}

void RayCastingInteractor::mousePressEvent(QMouseEvent* p_event)
{
    m_currentX = static_cast<qreal>(p_event->x());
    m_currentY = static_cast<qreal>(p_event->y());

    m_previousX = m_currentX;
    m_previousY = m_currentY;
}

void RayCastingInteractor::mouseMoveEvent(QMouseEvent* p_event)
{
    m_currentX = static_cast<qreal>(p_event->x());
    m_currentY = static_cast<qreal>(p_event->y());

    if (p_event->buttons() & Qt::LeftButton)
    {
        if (m_currentX != m_previousX || m_currentY != m_previousY)
        {
            rotateCamera();
        }
    }
    m_previousX = m_currentX;
    m_previousY = m_currentY;
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
    QVector3D va = arcballVector(m_previousX, m_previousY);
    QVector3D vb = arcballVector(m_currentX, m_currentY);

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
