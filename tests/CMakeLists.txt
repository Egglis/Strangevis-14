add_executable(geometryTest
    geometry.cpp
    ../geometry/edge.cpp
    ../geometry/quad.cpp
    ../geometry/cube.cpp
    ../geometry/plane.cpp
    ../geometry/cubeplaneintersection.cpp
    ../geometry/utils.cpp
)
target_link_libraries(geometryTest PRIVATE
    OpenGL32
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
)
add_test(Geometry geometryTest)
