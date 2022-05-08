#ifndef SLICINGPLANECONTROLS_H
#define SLICINGPLANECONTROLS_H
#include "../properties/cameraproperties.h"
#include "../properties/sharedproperties.h"
#include "../geometry/plane.h"

#include <QMatrix4x4>
#include <QVector4D>

// clang-format off
#include <imgui.h>
#include <ImGuizmo.h>
// clang-format on

class SlicingPlaneControls
{
  public:
    SlicingPlaneControls(std::shared_ptr<ISharedProperties> properties,
                         const CameraProperties& camera)
        : m_properties{properties}, m_camera{camera} {};
    void paint();

  private:
    void manipulateRotation();
    void manipulateTranslation();
    std::shared_ptr<ISharedProperties> m_properties;
    QMatrix4x4 m_rotationMatrix;
    QMatrix4x4 m_translationMatrix;
    const ImGuizmo::MODE m_mode{ImGuizmo::WORLD};
    const CameraProperties& m_camera;
    Plane m_clippingPlane{QVector4D{0,0,1,0}};
};

#endif // SLICINGPLANECONTROLS_H
