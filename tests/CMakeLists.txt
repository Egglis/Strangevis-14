add_executable(edge
    edge.cpp
    ../geometry/edge.cpp
)
target_link_libraries(edge PRIVATE
    OpenGL32
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
)
add_test(Geometry edge)
