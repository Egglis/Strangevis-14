#include "volumerenderer.h"

#include "../geometry.h"

VolumeRenderer::VolumeRenderer(
    const std::unique_ptr<ITextureStore>& textureStore,
    RenderSettings& settings, QVector4D& lpos, const CameraProperties& camera,
    QOpenGLExtraFunctions& openGLExtra, const ViewPort& viewPort)
    : m_textureStore{textureStore}, m_lpos{lpos}, m_renderSettings{settings},
      m_camera{camera}, m_openGLExtra{openGLExtra}, m_viewPort{viewPort}
{
}

void VolumeRenderer::paint()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setUniforms();
    bindTextures();

    Geometry::instance().bindCube();

    setAttributes();

    Geometry::instance().drawCube();

    m_textureStore->transferFunction().release();
    m_textureStore->volume().release();
    m_cubeProgram.release();
}

void VolumeRenderer::setUniforms()
{
    int location = -1;
    auto modelViewProjectionMatrix = m_camera.projectionViewMatrix() *
                                     m_textureStore->volume().modelMatrix();

    QVector3D rayOrigin =
        m_camera.viewMatrix().inverted().map(QVector3D(0, 0, 0));

    m_cubeProgram.bind();
    location = m_cubeProgram.uniformLocation("rayOrigin");
    m_cubeProgram.setUniformValue(location, rayOrigin);
    location = m_cubeProgram.uniformLocation("viewMatrix");
    m_cubeProgram.setUniformValue(location, m_camera.viewMatrix());
    location = m_cubeProgram.uniformLocation("modelMatrix");
    m_cubeProgram.setUniformValue(location,
                                  m_textureStore->volume().modelMatrix());
    location = m_cubeProgram.uniformLocation("modelViewProjectionMatrix");
    m_cubeProgram.setUniformValue(location, modelViewProjectionMatrix);

    location = m_cubeProgram.uniformLocation("focalLength");
    m_cubeProgram.setUniformValue(location, m_camera.focalLength());
    location = m_cubeProgram.uniformLocation("viewportSize");
    m_cubeProgram.setUniformValue(location, m_viewPort.viewPort());
    location = m_cubeProgram.uniformLocation("aspectRatio");
    m_cubeProgram.setUniformValue(location, m_viewPort.aspectRatio());

    auto [width, height, depth] = m_textureStore->volume().getDimensions();
    location = m_cubeProgram.uniformLocation("width");
    m_cubeProgram.setUniformValue(location, static_cast<int>(width));
    location = m_cubeProgram.uniformLocation("height");
    m_cubeProgram.setUniformValue(location, static_cast<int>(height));
    location = m_cubeProgram.uniformLocation("depth");
    m_cubeProgram.setUniformValue(location, static_cast<int>(depth));

    location = m_cubeProgram.uniformLocation("lpos");
    m_cubeProgram.setUniformValue(location, QVector3D(m_lpos.x(), m_lpos.y(), m_lpos.z()));

    for (const auto& [key, value] : m_renderSettings)
    {
        location = m_cubeProgram.uniformLocation(key);
        std::visit(
            [this, location](const auto& arg) {
                m_cubeProgram.setUniformValue(location, arg);
            },
            value);
    }
}

void VolumeRenderer::bindTextures()
{
    m_openGLExtra.glActiveTexture(GL_TEXTURE0);
    m_cubeProgram.setUniformValue("volumeTexture", 0);
    m_textureStore->volume().bind();

    m_openGLExtra.glActiveTexture(GL_TEXTURE1);
    m_cubeProgram.setUniformValue("transferFunction", 1);
    m_textureStore->transferFunction().bind();
}

void VolumeRenderer::setAttributes()
{
    int location = m_cubeProgram.attributeLocation("vertexPosition");
    m_cubeProgram.enableAttributeArray(location);
    m_cubeProgram.setAttributeBuffer(location, GL_FLOAT, 0, 3,
                                     sizeof(QVector3D));
}

void VolumeRenderer::compileShader()
{
    if (!m_cubeProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               ":shaders/cube-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_cubeProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               ":shaders/cube-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_cubeProgram.link())
        qDebug() << "Could not link shader program!";
}
