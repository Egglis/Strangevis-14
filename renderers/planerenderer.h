#ifndef PLANERENDERER_H
#define PLANERENDERER_H

#include "../properties/cameraproperties.h"
#include "../properties/rendersettingsproperties.h"
#include "../properties/sharedproperties.h"
#include "../texturestore.h"

#include <QOpenGLShaderProgram>

class PlaneRenderer
{
  public:
    PlaneRenderer(const std::unique_ptr<ITextureStore>& textureStore, const std::shared_ptr<const ISharedProperties> properties, const CameraProperties& camera, RenderSettings& renderSettings);
    void paint();
    void compileShader();

  private:
    QMatrix4x4 planeModelMatrix();
    const std::unique_ptr<ITextureStore>& m_textureStore;
    const CameraProperties& m_camera;
    QOpenGLShaderProgram m_planeProgram;
    RenderSettings& m_renderSettings;
    const std::shared_ptr<const ISharedProperties> m_properties;
};

#endif // PLANERENDERER_H
