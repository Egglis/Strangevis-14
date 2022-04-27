#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include "../properties/cameraproperties.h"
#include "../properties/viewport.h"
#include "../texturestore.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>

class VolumeRenderer
{
  public:
    VolumeRenderer(const std::unique_ptr<ITextureStore>& textureStore, const CameraProperties& camera, QOpenGLExtraFunctions& openGLextra, const ViewPort& viewPort);
    void paint();
    void compileShader();

  private:
    void setUniforms();
    void setAttributes();
    void bindTextures();

    QOpenGLShaderProgram m_cubeProgram;
    const CameraProperties& m_camera;
    const ViewPort& m_viewPort;
    const std::unique_ptr<ITextureStore>& m_textureStore;
    QOpenGLExtraFunctions& m_openGLExtra;
};

#endif // VOLUMERENDERER_H
