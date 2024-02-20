add_executable(fabricatio)

set(sources
    "GlAppWindow.cpp"
    "GlAppWindow.h"
    "Clock.cpp"
    "Clock.h"
    "ECSContainer.h"
    "main.cpp"
    "RenderSystem.cpp"
    "RenderSystem.h"
    "SpinSystem.h"
    "SpinSystem.cpp"
    "Transform.h"
    "IO.h"
    "IO.cpp"
    "MeshCatalog.h"
    "MeshCatalog.cpp"
)

if(LINUX)
    list(APPEND sources "NixRendererLoader.h" "NixRendererLoader.cpp")
elseif(WINDOWS)
    list(APPEND sources "Win32RendererLoader.h" "Win32RendererLoader.cpp")
endif()

target_sources(fabricatio
  PRIVATE
    ${sources}
    "NixRendererLoader.h"
    "NixRendererLoader.cpp"
)

target_link_libraries(fabricatio PRIVATE glfw glm::glm stb_image producentis)

add_custom_target(CopyRuntimeData
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/${Configuration}/shaders
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/${Configuration}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/${Configuration}/shaders
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/${Configuration}/assets
)
add_dependencies(fabricatio CopyRuntimeData)
