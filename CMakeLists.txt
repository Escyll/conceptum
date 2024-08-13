cmake_minimum_required(VERSION 3.17)

project(fabricatio)

set (CMAKE_CXX_STANDARD 23)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(vendor)
add_subdirectory(fundamentum)
add_subdirectory(producentis)
add_subdirectory(fabricatio)
