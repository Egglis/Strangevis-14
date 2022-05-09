#ifndef CAMERAPROPERTIES_H
#define CAMERAPROPERTIES_H

#include <QMatrix4x4>

class CameraProperties
{
  public:
    CameraProperties();
    QMatrix4x4 projectionMatrix() const { return m_projectionMatrix; };
    QMatrix4x4 rotationMatrix() const { return m_rotationMatrix; };
    QMatrix4x4 viewMatrix() const;
    QMatrix4x4 projectionViewMatrix() const
    {
        return projectionMatrix() * viewMatrix();
    };
    float zoomFactor() const { return m_zoom; };

    float focalLength() const { return m_focalLength; };
  public slots:
    void updateProjectionMatrix(float aspectRatio);
    void rotateCamera(float angle, QVector3D axis);
    void rotateCamera(QMatrix4x4 deltaMatrix);
    void moveCamera(QVector3D translation);
    void zoomCamera(float zoomFactor);

  private:
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_rotationMatrix;
    QVector3D m_translation;
    float m_zoom{1};

    constexpr static float m_nearPlane = 0.5;
    constexpr static float m_farPlane = 32.0;
    constexpr static float m_fov = 60.0;
    const float m_focalLength = 1.0 / qTan(M_PI / 180.0 * m_fov / 2.0);
};

#endif // CAMERAPROPERTIES_H
