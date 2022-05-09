#ifndef RAYCASTINGWIDGET_H
#define RAYCASTINGWIDGET_H

#include "../geometry/cubeplaneintersection.h"
#include "../geometry/plane.h"
#include "../properties/cameraproperties.h"
#include "../properties/gradientproperties.h"
#include "../properties/viewport.h"
#include "../properties/sharedproperties.h"
#include "../texturestore.h"
#include "planerenderer.h"
#include "volumerenderer.h"
#include "slicingplanecontrols.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QtImGui.h>

struct RenderProperties
{
    float zoomFactor;
    QVector3D cameraPosition;
    QString transferFunction;
    Plane clippingPlane;
    GradientMethod gradientMethod;
};

class RayCastingWidget : public QOpenGLWidget
{
    Q_OBJECT

  public:
    RayCastingWidget(RenderProperties initialProperties,
                     std::unique_ptr<ITextureStore>& textureStore,
                     std::shared_ptr<ISharedProperties>,
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
    void changeGradientMethod(GradientMethod method);
    void changeTransferFunction(QString transferFunctionName);

  private:
    void renderImGuizmo();

    QOpenGLExtraFunctions m_openGLExtra;
    std::unique_ptr<ITextureStore>& m_textureStore;
    QOpenGLShaderProgram m_cubeProgram;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_viewMatrix;
    GradientMethod m_gradientMethod;
    QString m_transferFunctionName;
    Plane m_clippingPlane;
    CubePlaneIntersection m_cubePlaneIntersection;
    CameraProperties m_camera;
    ViewPort m_viewPort;

    QtImGui::RenderRef m_imGuiReference;
    VolumeRenderer m_volumeRenderer;
    PlaneRenderer m_planeRenderer;
    SlicingPlaneControls m_slicingPlaneControls;

    qreal m_nearPlane = 0.5;
    qreal m_farPlane = 32.0;
    qreal m_fov = 60.0;
    const GLfloat m_focalLength = 1.0 / qTan(M_PI / 180.0 * m_fov / 2.0);
};

#endif // RAYCASTINGWIDGET_H
