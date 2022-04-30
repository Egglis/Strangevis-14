#include "planerenderer.h"

#include "../geometry.h"

#include <ImGui.h>
#include <ImGuizmo.h>

namespace
{
// We want to draw the plane the same size as the volume, plus a small margin.
// The margin should be the same on every side
// If a,b,c is scaling, and x,y,z is modelScale, this calculation ensures that
// a*x-x == b*y-y == c*z-z. These differences are the margins, so they're the
// same everywhere.
QVector3D planeScalingFactor(QVector3D modelScale)
{
    float baseScaling = static_cast<float>(sqrt(2));
    float minValue = std::min({modelScale.x(), modelScale.y(), modelScale.z()});
    QVector3D scaling = {
        minValue / modelScale.x() * (baseScaling - 1.0f) + 1.0f,
        minValue / modelScale.y() * (baseScaling - 1.0f) + 1.0f,
        minValue / modelScale.z() * (baseScaling - 1.0f) + 1.0f};
    return scaling * modelScale;
}
} // namespace

PlaneRenderer::PlaneRenderer(const std::unique_ptr<ITextureStore>& textureStore,
                             const CameraProperties& camera,
                             const CubePlaneIntersection& cubePlaneIntersection)
    : m_textureStore{textureStore}, m_camera{camera}, m_cubePlaneIntersection{
                                                          cubePlaneIntersection}
{
}

void PlaneRenderer::compileShader()
{
    if (!m_planeProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                ":shaders/plane-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_planeProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                ":shaders/plane-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_planeProgram.link())
        qDebug() << "Could not link shader program!";
}

void PlaneRenderer::paint()
{
    int location;
    m_planeProgram.bind();

    QMatrix4x4 modelViewProjectionMatrix = m_camera.projectionMatrix() *
                                           m_camera.viewMatrix() *
                                           planeModelMatrix();
    location = m_planeProgram.uniformLocation("modelViewProjectionMatrix");

    m_planeProgram.setUniformValue(location, modelViewProjectionMatrix);
    Geometry::instance().bindObliqueSliceIntersectionCoords();
    location = m_planeProgram.attributeLocation("vertexPosition");
    m_planeProgram.enableAttributeArray(location);
    m_planeProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                      sizeof(QVector3D));
    Geometry::instance().drawObliqueSlice();
    m_planeProgram.release();
}

QMatrix4x4 PlaneRenderer::planeModelMatrix()
{
    const auto& plane = m_cubePlaneIntersection.plane();
    QVector3D t = plane.d()*plane.normal();
    QMatrix4x4 modelMatrix{};
    auto scaleFactor = planeScalingFactor(
        m_textureStore->volume().scaleFactor());
    modelMatrix.scale(scaleFactor);

    modelMatrix.translate(-t);
    modelMatrix.translate(t/scaleFactor);

    return modelMatrix;
}
