#ifndef LIGTHRENDERER_H
#define LIGHTRENDERER_H

#include <QOpenGLShaderProgram>
#include "../geometry.h"
#include "../properties/cameraproperties.h"
#include <QVector3D>
#include "../properties/viewport.h"

class LightRenderer {
    public:
        LightRenderer(const CameraProperties& camera, const ViewPort& viewPort);
        void paint();
        void compileShader();
        void setLightTransform(QMatrix4x4 transform) {m_ligthTransform = transform;};
        QMatrix4x4 getLightTransform() {return m_ligthTransform; };
    private:
        const CameraProperties& m_camera;
        QOpenGLShaderProgram m_lightProgram;
        ViewPort m_viewPort;
        QMatrix4x4 m_ligthTransform;
};


#endif