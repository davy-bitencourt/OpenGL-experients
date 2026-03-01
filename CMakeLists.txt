cmake_minimum_required(VERSION 3.16)
project(opengl-experiments)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/glfw-3.4)

add_executable(programa 
    src/Main.cpp
)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(programa PRIVATE
    glfw
    GLEW::GLEW
    OpenGL::GL
)