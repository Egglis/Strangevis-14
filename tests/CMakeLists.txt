add_executable(geometryTest
    geometry.cpp
    ../geometry/edge.cpp
    ../geometry/quad.cpp
    ../geometry/cube.cpp
    ../geometry/plane.cpp
    ../geometry/cubeplaneintersection.cpp
    ../geometry/utils.cpp
    ../geometry/box.cpp
)
target_link_libraries(geometryTest PRIVATE
    OpenGL32
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
)
# Some files require Qt-dlls, but since the test is not a Qt application, windeployqt doesn't work
# We manually copy the required dlls from building the main application into the tests folder as
# a rather bad workaround
add_custom_command(TARGET geometryTest POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/../Qt6Cored.dll
        ${CMAKE_CURRENT_BINARY_DIR}/Qt6Cored.dll)
add_custom_command(TARGET geometryTest POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/../Qt6Guid.dll
        ${CMAKE_CURRENT_BINARY_DIR}/Qt6Guid.dll)
add_test(Geometry geometryTest)
