#ifndef IMGUIZMORENDERER_H
#define IMGUIZMORENDERER_H

#include "../properties/cameraproperties.h"
#include "../properties/sharedproperties.h"
#include "slicingplanecontrols.h"

#include <QOpenGLWidget>
#include <QTimer>
#include <QtImGui.h>

class ImguizmoWidget : public QOpenGLWidget
{
    Q_OBJECT
  public:
    ImguizmoWidget(std::shared_ptr<ISharedProperties>, CameraProperties& camera, QWidget* renderSettings, QWidget* lightSettings,
                   QWidget* parent = nullptr,
                   Qt::WindowFlags f = Qt::WindowFlags());

  signals:
    void updateScene();

  protected:
    virtual void initializeGL();
    virtual void paintGL();

  private:
    void renderImGuizmo();
    QtImGui::RenderRef m_imGuiReference;
    CameraProperties& m_camera;
    SlicingPlaneControls m_slicingPlaneControls;
    QTimer* m_refreshTimer;
};
#endif // IMGUIZMORENDERER_H
