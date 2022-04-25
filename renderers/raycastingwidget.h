#ifndef RAYCASTINGWIDGET_H
#define RAYCASTINGWIDGET_H

#include "../geometry/cubeplaneintersection.h"
#include "../geometry/plane.h"
#include "../properties/gradientproperties.h"
#include "../texturestore.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include <QtImGui.h>

enum class Projection { Orthographic, Perspective };

struct RenderProperties
{
    float zoomFactor;
    QString transferFunction;
    Plane clippingPlane;
    GradientMethod gradientMethod;
    Projection projectionMode;
};

class RayCastingWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

  public:
    RayCastingWidget(RenderProperties initialProperties,
                     std::unique_ptr<ITextureStore>& textureStore,
                     QWidget* parent = nullptr,
                     Qt::WindowFlags f = Qt::WindowFlags());

  protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
  protected slots:
    void rotateCamera(qreal angle, QVector3D axis);
    void zoomCamera(float zoomFactor);
    void updateClippingPlane(Plane clippingPlane);
    void changeProjectionMode(Projection projectionMode);
    void changeGradientMethod(GradientMethod method);
    void changeTransferFunction(QString transferFunctionName);

  private:
    void renderImGuizmo();

    std::unique_ptr<ITextureStore>& m_textureStore;
    QOpenGLShaderProgram m_cubeProgram;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_viewMatrix;
    GradientMethod m_gradientMethod;
    Projection m_projectionMode;
    QString m_transferFunctionName;
    Plane m_clippingPlane;
    CubePlaneIntersection m_cubePlaneIntersection;

    QtImGui::RenderRef m_imGuiReference;

    qreal m_nearPlane = 0.5;
    qreal m_farPlane = 32.0;
    qreal m_fov = 60.0;
    const GLfloat m_focalLength = 1.0 / qTan(M_PI / 180.0 * m_fov / 2.0);
    constexpr static QVector4D DISABLED_CLIPPING_EQUATION =
        QVector4D{0, 0, 1, 1000};
};

#endif // RAYCASTINGWIDGET_H
