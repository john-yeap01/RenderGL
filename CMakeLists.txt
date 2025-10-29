cmake_minimum_required(VERSION 3.14)
project(OpenGLBinaryStyle)

set(CMAKE_CXX_STANDARD 17)

# Add include paths
include_directories(
    ${CMAKE_SOURCE_DIR} 
    lib/glad/include
    lib/glfw/include
    lib/stb/include
)

# Add GLAD source
add_library(glad STATIC lib/glad/src/glad.c)

# Link directory where libglfw3.a lives
link_directories(${CMAKE_SOURCE_DIR}/lib/glfw/lib)

# --- build stb_image as its own STATIC library ---
add_library(stb_image STATIC
    lib/stb/src/stb_image.cpp
)
target_include_directories(stb_image PUBLIC
    lib/stb/include
)

file(COPY ${CMAKE_SOURCE_DIR}/assets
     DESTINATION ${CMAKE_BINARY_DIR})

# Create main app
add_executable(OpenGLBinaryStyle 
    main.cpp
    shaderClass.cpp
    VBO.cpp
    VAO.cpp
    EBO.cpp
)

# Link all required libs
target_link_libraries(OpenGLBinaryStyle
    glad
    glfw3
    stb_image
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreFoundation"
)