#ifndef CAMERAPROPERTIES_H
#define CAMERAPROPERTIES_H

#include <QMatrix4x4>

class CameraProperties
{
  public:
    CameraProperties();
    QMatrix4x4 projectionMatrix() const { return m_projectionMatrix; };
    QMatrix4x4 rotationMatrix() const { return m_rotationMatrix; };
    QMatrix4x4 viewMatrix() const
    {
        return m_viewMatrix;
    };
    QMatrix4x4 projectionViewMatrix() const
    {
        return projectionMatrix() * viewMatrix();
    };
  public slots:
    void updateProjectionMatrix(float aspectRatio);
    void rotateCamera(float angle, QVector3D axis);
    void moveCamera(QVector3D translation);
    void zoomCamera(float zoomFactor);

  private:
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_rotationMatrix;
    QMatrix4x4 m_viewMatrix;

    constexpr static float m_nearPlane = 0.5;
    constexpr static float m_farPlane = 32.0;
    constexpr static float m_fov = 60.0;
};

#endif // CAMERAPROPERTIES_H
