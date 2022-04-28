#include "cameraproperties.h"

#include <QVector3D>

CameraProperties::CameraProperties()
{
    updateProjectionMatrix(16.0f / 9.0f);
}

void CameraProperties::updateProjectionMatrix(float aspectRatio)
{
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fov, aspectRatio, m_nearPlane, m_farPlane);
}

void CameraProperties::rotateCamera(float angle, QVector3D axis)
{

    QMatrix4x4 inverseViewMatrix = m_viewMatrix.inverted();
    QVector4D transformedAxis = inverseViewMatrix * QVector4D(axis,
    0.0f);
    m_viewMatrix.rotate(angle,
    transformedAxis.toVector3D());
    m_rotationMatrix.rotate(angle, transformedAxis.toVector3D());

}

void CameraProperties::moveCamera(QVector3D translation)
{
    m_viewMatrix.translate(translation);
}

void CameraProperties::zoomCamera(float zoomFactor)
{
    m_viewMatrix.scale(zoomFactor);
}
