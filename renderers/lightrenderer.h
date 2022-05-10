#ifndef LIGTHRENDERER_H
#define LIGHTRENDERER_H

#include "../geometry.h"
#include "../properties/cameraproperties.h"
#include "../properties/rendersettingsproperties.h"
#include "../properties/viewport.h"

#include <QVector3D>
#include <QOpenGLShaderProgram>

class LightRenderer {
    public:
        LightRenderer(const CameraProperties& camera, const ViewPort& viewPort, RenderSettings& renderSettings);
        void paint();
        void compileShader();
        void setLightTransform(QMatrix4x4 transform) {m_ligthTransform = transform;};
        QMatrix4x4 getLightTransform() {return m_ligthTransform; };
    private:
        const CameraProperties& m_camera;
        RenderSettings& m_renderSettings;
        QOpenGLShaderProgram m_lightProgram;
        ViewPort m_viewPort;
        QMatrix4x4 m_ligthTransform;
};


#endif