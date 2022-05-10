#include "slicingplanecontrols.h"

namespace
{
constexpr float G_SIZE = 4.f; // Scales the size of the gizmo
constexpr int G_AREA = 256;   // Size of the area the gizmos are rendered in.
constexpr float SNAP_ANGLE = 15.f;
constexpr float SNAP_T = 0.02f;
constexpr float SNAP_TRANSLATION[3]{SNAP_T, SNAP_T, SNAP_T};
constexpr ImGuizmo::MODE MODE{ImGuizmo::WORLD};
} // namespace

void SlicingPlaneControls::paint()
{
    manipulateRotation();
    manipulateTranslation();
}

void SlicingPlaneControls::manipulateRotation()
{
    auto io = ImGui::GetIO();
    auto [w, h] = io.DisplaySize;
    ImGuizmo::SetRect(w - G_AREA, h - G_AREA, G_AREA, G_AREA);
    QMatrix4x4 deltaMatrix{};

    if (ImGuizmo::Manipulate(
            ((G_SIZE / m_camera.zoomFactor()) * m_camera.viewMatrix())
                .constData(),
            m_camera.projectionMatrix().constData(), ImGuizmo::ROTATE, MODE,
            m_rotationMatrix.data(), deltaMatrix.data(), &SNAP_ANGLE)

    )
    {
        float translation[3];
        float rotation[3];
        float scale[3];
        ImGuizmo::DecomposeMatrixToComponents(deltaMatrix.data(), translation,
                                              rotation, scale);

        auto clippingPlane = m_properties->clippingPlane().plane();
        clippingPlane.rotate(rotation[0], QVector3D{1, 0, 0});
        clippingPlane.rotate(rotation[1], QVector3D{0, 1, 0});
        clippingPlane.rotate(rotation[2], QVector3D{0, 0, 1});

        m_properties->clippingPlane().updateClippingPlane(clippingPlane);
    }
}

void SlicingPlaneControls::manipulateTranslation()
{
    auto io = ImGui::GetIO();
    auto [w, h] = io.DisplaySize;
    ImGuizmo::SetRect(w - G_AREA, h - 2 * G_AREA, G_AREA, G_AREA);
    QMatrix4x4 deltaMatrix{};

    if (ImGuizmo::Manipulate(
            ((G_SIZE / m_camera.zoomFactor()) * m_camera.viewMatrix())
                .constData(),
            m_camera.projectionMatrix().constData(), ImGuizmo::TRANSLATE, MODE,
            m_translationMatrix.data(), deltaMatrix.data(), SNAP_TRANSLATION)

    )
    {
        float translation[3];
        float rotation[3];
        float scale[3];
        ImGuizmo::DecomposeMatrixToComponents(deltaMatrix.data(), translation,
                                              rotation, scale);

        auto clippingPlane = m_properties->clippingPlane().plane();
        QVector3D trans{translation[0], translation[1], translation[2]};
        clippingPlane.translate(trans);

        m_properties->clippingPlane().updateClippingPlane(clippingPlane);
    }
    if (ImGuizmo::IsUsing())
    {
        m_resetTranslationMatrixTimer.start();
    }
}
