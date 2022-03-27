cmake_minimum_required(VERSION 3.16)

project(strangevis VERSION 1.0.0 LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS REQUIRED
    Core
    Gui
    Widgets
    OpenGLWidgets
    Xml
    )
# if (MSVC)
#     # warning level 4 and all warnings as errors
#     add_compile_options(/Qpar /Qpar-report:2 /Qvec-report:1)
# else()
#     # lots of warnings and all warnings as errors
#     add_compile_options(-Wall -Wextra -pedantic -Werror)
# endif()

# Set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Include Qt deployment helper function
include(windeployqt)

qt_add_executable(strangevis WIN32
    main.cpp
    application.cpp
    mainwindow.cpp
    geometry.cpp
    texturestore.cpp
    volume.cpp
    transfertexture.cpp
    renderers/raycastingwidget.cpp
    properties/sharedproperties.cpp
    properties/clippingplaneproperties.cpp
    properties/gradientproperties.cpp
    properties/transferproperties.cpp
    ui/renderwidget.cpp
    ui/transferfunctionwidget.cpp
    ui/rectangulargridlayout.cpp
    ui/obliqueslicewidget.cpp
    ui/parameterwidget.cpp
    ui/mainwindowwidget.cpp
    geometry/cubeplaneintersection.cpp
    geometry/cube.cpp
    geometry/edge.cpp
    geometry/plane.cpp
    geometry/utils.cpp
    geometry/quad.cpp
)

set(shaders_resource_files
    "shaders/cube-fs.glsl"
    "shaders/cube-vs.glsl"
    "shaders/slice-fs.glsl"
    "shaders/slice-vs.glsl"
)

qt6_add_resources(strangevis "shaders"
    PREFIX
        "/shaders"
    BASE
        "shaders"
    FILES
        ${shaders_resource_files}
)

qt6_add_resources(strangevis "cmaps"
    PREFIX
        "/cmaps"
    BASE
        "res"
    FILES
        "res/cmaps.xml"
)

target_link_libraries(strangevis PRIVATE
    OpenGL32
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::Xml
)
windeployqt(strangevis)

add_subdirectory(tests)
