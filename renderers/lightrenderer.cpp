#include "lightrenderer.h"

LightRenderer::LightRenderer(const CameraProperties& camera,
                             RenderSettings& renderSettings)
    : m_camera{camera}, m_renderSettings{renderSettings}
{
}

void LightRenderer::compileShader()
{
    if (!m_lightProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                ":shaders/light-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_lightProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                ":shaders/light-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_lightProgram.link())
        qDebug() << "Could not link shader program!";
}

void LightRenderer::paint()
{
    int location;
    m_lightProgram.bind();

    QMatrix4x4 modelViewProjectionMatrix = m_camera.projectionMatrix() *
                                           m_camera.viewMatrix() *
                                           m_ligthTransform.inverted();

    location = m_lightProgram.uniformLocation("modelViewProjectionMatrix");
    m_lightProgram.setUniformValue(location, modelViewProjectionMatrix);

    location = m_lightProgram.uniformLocation("headLight");
    std::visit(
        [this, location](const auto& arg) {
            m_lightProgram.setUniformValue(location, arg);
        },
        m_renderSettings["headLight"]);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    Geometry::instance().bindLightSource();
    location = m_lightProgram.attributeLocation("position");
    m_lightProgram.enableAttributeArray(location);
    m_lightProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                      sizeof(QVector3D));
    Geometry::instance().drawLightSource();
    m_lightProgram.release();

    glDisable(GL_PROGRAM_POINT_SIZE);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}
