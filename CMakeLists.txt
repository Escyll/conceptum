cmake_minimum_required(VERSION 3.17)

project(fabricatio)

set (CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

add_subdirectory(vendor)
add_subdirectory(fabricatio)