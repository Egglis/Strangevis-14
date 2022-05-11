#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include "../properties/cameraproperties.h"
#include "../properties/rendersettingsproperties.h"
#include "../properties/viewport.h"
#include "lightrenderer.h"
#include "../texturestore.h"
#include "../geometry/plane.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

class VolumeRenderer
{
  public:
    VolumeRenderer(const std::unique_ptr<ITextureStore>& textureStore,
                   RenderSettings& settings,
                   const CameraProperties& camera,
                   QOpenGLExtraFunctions& openGLextra,
                   const ViewPort& viewPort,
                   const Plane& plane,
                   LightRenderer& lightRenderer
                   );
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
    RenderSettings& m_renderSettings;
    LightRenderer& m_lightRenderer;
    const Plane& m_plane;
};
#endif // VOLUMERENDERER_H
