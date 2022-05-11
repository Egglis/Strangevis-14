#include "imguizmorenderer.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

// clang-format off
#include <ImGui.h>
#include <ImGuizmo.h>
// clang-format on
ImguizmoWidget::ImguizmoWidget(std::shared_ptr<ISharedProperties> properties,
                               CameraProperties& camera, QWidget* renderSettingsWidget, QWidget* lightSettingsWidget, QWidget* parent,
                               Qt::WindowFlags f)
    : QOpenGLWidget{parent, f}, m_camera{camera}, m_imGuiReference{nullptr},
      m_slicingPlaneControls{properties, m_camera}

{
    setMouseTracking(true);
    m_refreshTimer = new QTimer();
    m_refreshTimer->setInterval(1.f/60);
    connect(m_refreshTimer, &QTimer::timeout, [this](){update();});
    m_refreshTimer->start();

    auto* hLayout = new QHBoxLayout(this);
    auto* leftColumn = new QVBoxLayout();
    leftColumn->addSpacing(256);
    leftColumn->addStretch(3);
    leftColumn->addWidget(renderSettingsWidget,1);
    hLayout->addLayout(leftColumn);
    hLayout->addStretch(3);

    auto* rightColumn = new QVBoxLayout();
    rightColumn->addWidget(lightSettingsWidget, 1);
    rightColumn->addStretch(3);
    rightColumn->addSpacing(2*256);
    hLayout->addLayout(rightColumn);
}

void ImguizmoWidget::initializeGL()
{
    m_imGuiReference = QtImGui::initialize(this, false);
}

void ImguizmoWidget::paintGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImGuizmo();
}

void ImguizmoWidget::renderImGuizmo()
{
    QtImGui::newFrame(m_imGuiReference);

    ImGuizmo::BeginFrame();
    ImGuizmo::Enable(true);
    auto rotationMatrix = m_camera.rotationMatrix();
    ImGuizmo::ViewManipulate(rotationMatrix.data(), 2.0f * sqrt(3.0f),
                             ImVec2(0, 0), ImVec2(128, 128), 0);
    if (m_camera.rotationMatrix() != rotationMatrix)
    {
        m_camera.rotateCamera(rotationMatrix);
        emit updateScene();
    }
    m_slicingPlaneControls.paint();
    ImGui::Render();
    QtImGui::render(m_imGuiReference);
}
