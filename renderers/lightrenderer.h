#ifndef LIGTHRENDERER_H
#define LIGHTRENDERER_H

#include "../geometry.h"
#include "../properties/cameraproperties.h"
#include "../properties/rendersettingsproperties.h"

#include <QOpenGLShaderProgram>
#include <QVector3D>

class LightRenderer
{
  public:
    LightRenderer(const CameraProperties& camera,
                  RenderSettings& renderSettings);
    void paint();
    void compileShader();
    void setLightTransform(QMatrix4x4 transform)
    {
        m_ligthTransform = transform;
    };
    QMatrix4x4 getLightTransform() { return m_ligthTransform; };

  private:
    const CameraProperties& m_camera;
    RenderSettings& m_renderSettings;
    QOpenGLShaderProgram m_lightProgram;
    QMatrix4x4 m_ligthTransform;
};

#endif