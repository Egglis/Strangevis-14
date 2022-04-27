cmake_minimum_required(VERSION 3.16)

project(strangevis VERSION 1.0.0 LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Include and run the conan package manager to install some external libraries for us
# Requires Python or Conan on the path
include(conan)
conan_cmake_autodetect(settings)
conan_cmake_install(
    PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings}
    )

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
# Include Qt deployment helper function
include(windeployqt)

# Include the external qtimgui library to allow us to render ImGui widgets in QOpenGL
# This was not available in conan.
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/qtimgui)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS REQUIRED
    Core
    Gui
    Widgets
    OpenGLWidgets
    Xml
    Charts
    )

if (MSVC)
    add_compile_options(/W4 /external:anglebrackets /external:W0 /external:I "${CMAKE_SOURCE_DIR}/vendor")
endif()

qt_add_executable(strangevis WIN32
    main.cpp
    application.cpp
    mainwindow.cpp
    geometry.cpp
    texturestore.cpp
    volume.cpp
    transferfunction.cpp
    transfertexture.cpp
    renderers/raycastingwidget.cpp
    renderers/obliqueslicewidget.cpp
    renderers/volumerenderer.cpp
    properties/cameraproperties.cpp
    properties/sharedproperties.cpp
    properties/clippingplaneproperties.cpp
    properties/gradientproperties.cpp
    properties/transferproperties.cpp
    ui/renderwidget.cpp
    ui/rectangulargridlayout.cpp
    ui/obliquesliceinteractor.cpp
    ui/parameterwidget.cpp
    ui/mainwindowwidget.cpp
    ui/histogramwidget.cpp
    ui/transferwidget/transferfunctiongraph.cpp
    ui/transferwidget/transferfunctionwidget.cpp
    ui/transferwidget/hintitem.cpp
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
    Qt6::Charts
    qt_imgui_widgets
    ${CONAN_LIBS}
)
windeployqt(strangevis)

add_subdirectory(tests)
