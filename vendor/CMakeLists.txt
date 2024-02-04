project(vendor)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glm-0.9.9.8)
add_subdirectory(glfw-3.3.9)
add_subdirectory(glad)
add_subdirectory(stb_image)