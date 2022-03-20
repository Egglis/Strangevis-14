cmake_minimum_required(VERSION 3.16)

project(strangevis VERSION 1.0.0 LANGUAGES CXX)

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
    )


qt_add_executable(strangevis WIN32
    main.cpp
    mainwindow.cpp
    geometry.cpp
    environment.cpp
    volume.cpp
    properties/sharedproperties.cpp
    ui/renderwidget.cpp
    ui/rectangulargridlayout.cpp
    ui/parameterwidget.cpp
    ui/mainwindowwidget.cpp
    geometry/cube.cpp
    geometry/edge.cpp
    geometry/plane.cpp
    geometry/quad.cpp
)

set(shaders_resource_files
    "shaders/cube-fs.glsl"
    "shaders/cube-vs.glsl"
)

qt6_add_resources(strangevis "shaders"
    PREFIX
        "/shaders"
    FILES
    ${shaders_resource_files}
)

target_link_libraries(strangevis PRIVATE
    OpenGL32
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
)
