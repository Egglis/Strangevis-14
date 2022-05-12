#include "raycastingwidget.h"

#include "../geometry.h"

RayCastingWidget::RayCastingWidget(
    RenderProperties initialRenderProperties,
    std::unique_ptr<ITextureStore>& textureStore,
    std::shared_ptr<ISharedProperties> properties, CameraProperties& camera,
    QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f), m_textureStore{textureStore},
      m_renderSettings{initialRenderProperties.renderSettings},
      m_transferFunctionName{initialRenderProperties.transferFunction},
      m_clippingPlane{initialRenderProperties.clippingPlane},
      m_cubePlaneIntersection{initialRenderProperties.clippingPlane},
      m_viewPort{width(), height()}, m_camera{camera},
      m_volumeRenderer{textureStore,
                       m_renderSettings,
                       m_camera,
                       m_openGLExtra,
                       m_viewPort,
                       m_lightRenderer,
                       m_cubePlaneIntersection.plane()},
      m_lightRenderer{m_camera, m_renderSettings}, m_planeRenderer{
                                                       textureStore, m_camera,
                                                       m_renderSettings}
{
    m_camera.moveCamera(initialRenderProperties.cameraPosition);
    m_camera.zoomCamera(initialRenderProperties.zoomFactor);

    connect(&m_textureStore->volume(), &Volume::volumeLoaded, this,
            [this]() { update(); });
}

void RayCastingWidget::rotateCamera(qreal angle, QVector3D axis)
{
    m_camera.rotateCamera(qRadiansToDegrees(angle), axis);
    updateLightTransformMatrix();
    update();
}
void RayCastingWidget::zoomCamera(float zoomFactor)
{
    m_camera.zoomCamera(zoomFactor);
    updateLightTransformMatrix();
    update();
}

void RayCastingWidget::moveLightSource(QVector3D vb)
{
    m_lightTranslation.setToIdentity();
    m_lightTranslation.translate(-0.5f * vb * (2.0f * sqrt(3.0f)));
    updateLightTransformMatrix();
}

void RayCastingWidget::updateLightTransformMatrix()
{
    const QMatrix4x4 lightTransformMatrix = m_camera.viewMatrix().inverted() *
                                            m_lightTranslation *
                                            m_camera.viewMatrix();

    m_lightRenderer.setLightTransform(lightTransformMatrix);
    update();
}

void RayCastingWidget::initializeGL()
{
    m_openGLExtra.initializeOpenGLFunctions();

    // initialize geometry
    Geometry::instance();
    m_volumeRenderer.compileShader();
    m_planeRenderer.compileShader();
    m_lightRenderer.compileShader();
}

void RayCastingWidget::resizeGL(int w, int h)
{
    qreal aspectRatio = static_cast<qreal>(w) / static_cast<qreal>(h);

    m_camera.updateProjectionMatrix(aspectRatio);
    m_viewPort.updateViewPort(w, h);
}

void RayCastingWidget::paintGL()
{
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_textureStore->volume().loadingInProgress())
        return;
    m_volumeRenderer.paint();
    m_planeRenderer.paint();
    m_lightRenderer.paint();
}

void RayCastingWidget::updateClippingPlane(Plane clippingPlane)
{
    m_clippingPlane = clippingPlane;
    m_cubePlaneIntersection.changePlane(clippingPlane);
    update();
}

void RayCastingWidget::changeTransferFunction(QString transferFunction)
{
    m_transferFunctionName = transferFunction;
    update();
}

void RayCastingWidget::changeRenderSettings(RenderSettings renderSettings)
{
    m_renderSettings = renderSettings;
    update();
}
