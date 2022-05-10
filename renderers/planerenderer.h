#ifndef PLANERENDERER_H
#define PLANERENDERER_H

#include "../properties/cameraproperties.h"
#include "../texturestore.h"

#include <QOpenGLShaderProgram>

class PlaneRenderer
{
  public:
    PlaneRenderer(const std::unique_ptr<ITextureStore>& textureStore, const CameraProperties& camera);
    void paint();
    void compileShader();

  private:
    QMatrix4x4 planeModelMatrix();
    const std::unique_ptr<ITextureStore>& m_textureStore;
    const CameraProperties& m_camera;
    QOpenGLShaderProgram m_planeProgram;
};

#endif // PLANERENDERER_H