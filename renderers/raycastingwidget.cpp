#include "raycastingwidget.h"

#include "../geometry.h"

// clang-format off
#include <ImGui.h>
#include <ImGuizmo.h>
// clang-format on

RayCastingWidget::RayCastingWidget(
    RenderProperties initialRenderProperties,
    std::unique_ptr<ITextureStore>& textureStore,
    std::shared_ptr<ISharedProperties> properties, QWidget* parent,
    Qt::WindowFlags f)
    : QOpenGLWidget(parent, f), m_textureStore{textureStore},
      m_transferFunctionName{initialRenderProperties.transferFunction},
      m_clippingPlane{initialRenderProperties.clippingPlane},
      m_cubePlaneIntersection{initialRenderProperties.clippingPlane},
      m_imGuiReference{nullptr}, m_viewPort{width(), height()},
      m_volumeRenderer{textureStore, m_renderSettings, m_camera, m_openGLExtra,
                       m_viewPort} m_planeRenderer{textureStore, m_camera},
      m_slicingPlaneControls{properties, m_camera}
{
    m_camera.moveCamera(initialRenderProperties.cameraPosition);
    m_camera.zoomCamera(initialRenderProperties.zoomFactor);

    connect(&m_textureStore->volume(), &Volume::volumeLoaded, this,
            [this]() { update(); });
}

void RayCastingWidget::rotateCamera(qreal angle, QVector3D axis)
{
    m_camera.rotateCamera(qRadiansToDegrees(angle), axis);
    update();
}
void RayCastingWidget::zoomCamera(float zoomFactor)
{
    m_camera.zoomCamera(zoomFactor);
    update();
}

void RayCastingWidget::initializeGL()
{
    m_openGLExtra.initializeOpenGLFunctions();
    m_imGuiReference = QtImGui::initialize(this, false);

    // initialize geometry
    Geometry::instance();
    m_volumeRenderer.compileShader();
    m_planeRenderer.compileShader();
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
    renderImGuizmo();
    m_volumeRenderer.paint();
    m_planeRenderer.paint();
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

void RayCastingWidget::renderImGuizmo()
{
    QtImGui::newFrame(m_imGuiReference);

    ImGuizmo::BeginFrame();
    ImGuizmo::Enable(true);
    auto rotationMatrix = m_camera.rotationMatrix();
    ImGuizmo::ViewManipulate(rotationMatrix.data(), 2.0f * sqrt(3.0f),
                             ImVec2(0, 0), ImVec2(128, 128), 0);
    m_camera.rotateCamera(rotationMatrix);
    m_slicingPlaneControls.paint();
    ImGui::Render();
    QtImGui::render(m_imGuiReference);
}
