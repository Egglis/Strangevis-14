#include "cameraproperties.h"

#include <QVector3D>

CameraProperties::CameraProperties() { updateProjectionMatrix(16.0f / 9.0f); }

void CameraProperties::updateProjectionMatrix(float aspectRatio)
{
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fov, aspectRatio, m_nearPlane, m_farPlane);

    m_orthoProjectionMatrix.setToIdentity();
    m_orthoProjectionMatrix.ortho(-2.0f*aspectRatio, 2.0f*aspectRatio, -2.0, 2.0f, m_nearPlane, m_farPlane);
}

void CameraProperties::rotateCamera(float angle, QVector3D axis)
{

    QMatrix4x4 inverseViewMatrix = viewMatrix().inverted();
    QVector4D transformedAxis = inverseViewMatrix * QVector4D(axis,
    0.0f);
    m_rotationMatrix.rotate(angle, transformedAxis.toVector3D());

}

void CameraProperties::rotateCamera(QMatrix4x4 rotationMatrix)
{
    m_rotationMatrix = rotationMatrix;
    m_rotationMatrix.setColumn(3,QVector4D{0,0,0,1});
}

void CameraProperties::moveCamera(QVector3D translation)
{
    m_translation += translation;
}

void CameraProperties::zoomCamera(float zoomFactor)
{
    m_zoom *= zoomFactor;
}

QMatrix4x4 CameraProperties::viewMatrix() const
{
    QMatrix4x4 view{};
    view.translate(m_translation);
    view *= m_rotationMatrix;
    view.scale(m_zoom);
    return view;
}
