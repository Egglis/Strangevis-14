#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include "../properties/cameraproperties.h"
#include "../properties/rendersettingsproperties.h"
#include "../properties/viewport.h"
#include "../texturestore.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

class VolumeRenderer
{
  public:
    VolumeRenderer(const std::unique_ptr<ITextureStore>& textureStore,
                   RenderSettings& settings,
                   QVector4D& lpos,
                   const CameraProperties& camera,
                   QOpenGLExtraFunctions& openGLextra,
                   const ViewPort& viewPort);
    void paint();
    void compileShader();

  private:
    void setUniforms();
    void setAttributes();
    void bindTextures();

    QOpenGLShaderProgram m_cubeProgram;
    const CameraProperties& m_camera;
    QVector4D& m_lpos;
    const ViewPort& m_viewPort;
    const std::unique_ptr<ITextureStore>& m_textureStore;
    QOpenGLExtraFunctions& m_openGLExtra;
    RenderSettings& m_renderSettings;
};

#endif // VOLUMERENDERER_H
