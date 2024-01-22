cmake_minimum_required(VERSION 3.17)

project(RayTracer)

set (CMAKE_CXX_STANDARD 23)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

include_directories(stb_image)

add_subdirectory(glfw-3.3.9)
add_subdirectory(glad)
add_subdirectory(src)