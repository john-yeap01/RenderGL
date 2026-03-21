cmake_minimum_required(VERSION 3.14)
project(OpenGLBinaryStyle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RENDERGL_IMGUI_DIR ${CMAKE_SOURCE_DIR}/3rdparty/imgui)
set(RENDERGL_HAS_IMGUI OFF)
if(EXISTS ${RENDERGL_IMGUI_DIR}/imgui.cpp)
    set(RENDERGL_HAS_IMGUI ON)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    lib/glad/include
    lib/glfw/include
    lib/stb/include
    lib
)

add_library(glad STATIC lib/glad/src/glad.c)

add_library(stb_image STATIC
    lib/stb/src/stb_image.cpp
)
target_include_directories(stb_image PUBLIC
    lib/stb/include
)

set(RENDERGL_SOURCES
    src/Main.cpp
    src/Camera.cpp
    src/EBO.cpp
    src/Texture.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/shaderClass.cpp
    src/platform/Application.cpp
    src/view/ControlPanelView.cpp
    src/view/SceneRenderer.cpp
    src/viewmodel/SceneViewModel.cpp
)

if(RENDERGL_HAS_IMGUI)
    list(APPEND RENDERGL_SOURCES
        3rdparty/imgui/imgui.cpp
        3rdparty/imgui/imgui_widgets.cpp
        3rdparty/imgui/imgui_tables.cpp
        3rdparty/imgui/imgui_draw.cpp
        3rdparty/imgui/imgui_demo.cpp
        3rdparty/imgui/backends/imgui_impl_glfw.cpp
        3rdparty/imgui/backends/imgui_impl_opengl3.cpp
    )
endif()

add_executable(OpenGLBinaryStyle ${RENDERGL_SOURCES})

target_include_directories(OpenGLBinaryStyle PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

if(RENDERGL_HAS_IMGUI)
    target_include_directories(OpenGLBinaryStyle PRIVATE
        3rdparty/imgui
        3rdparty/imgui/backends
    )
    target_compile_definitions(OpenGLBinaryStyle PRIVATE RENDERGL_HAS_IMGUI=1)
endif()

target_compile_definitions(OpenGLBinaryStyle PRIVATE
    RENDERGL_RESOURCE_DIR="${CMAKE_SOURCE_DIR}/resources"
)

find_package(OpenGL REQUIRED)

if(APPLE)
    link_directories(${CMAKE_SOURCE_DIR}/lib/glfw/lib)
    target_link_libraries(OpenGLBinaryStyle
        glad
        glfw3
        stb_image
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreFoundation"
    )
else()
    find_package(glfw3 QUIET)
    if(glfw3_FOUND)
        target_link_libraries(OpenGLBinaryStyle glad glfw stb_image OpenGL::GL)
    else()
        target_link_libraries(OpenGLBinaryStyle glad stb_image OpenGL::GL)
    endif()
endif()

file(COPY ${CMAKE_SOURCE_DIR}/resources/assets DESTINATION ${CMAKE_BINARY_DIR})
