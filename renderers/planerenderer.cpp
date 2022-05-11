#include "planerenderer.h"

#include "../geometry.h"

PlaneRenderer::PlaneRenderer(const std::unique_ptr<ITextureStore>& textureStore,
                             const CameraProperties& camera,
                             RenderSettings& renderSettings)
    : m_textureStore{textureStore}, m_camera{camera}, m_renderSettings{
                                                          renderSettings}
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

    m_planeProgram.setUniformValue("showSlice", std::get<bool>(m_renderSettings["showSlice"]));

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
    QMatrix4x4 modelMatrix{};
    modelMatrix.scale(m_textureStore->volume().scaleFactor());
    return modelMatrix;
}
