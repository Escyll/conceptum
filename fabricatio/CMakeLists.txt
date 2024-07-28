include(CMakePrintHelpers)

add_executable(fabricatio)

set(sources
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
    "ScalarGrid.h"
    "ScalarGrid.cpp"
    "MarchingCubes.h"
    "MarchingCubes.cpp"
    "InputSystem.h"
    "InputSystem.cpp"
)

target_sources(fabricatio
  PRIVATE
    ${sources}
)

target_link_libraries(fabricatio PRIVATE glm::glm stb_image FastNoiseLite producentis)

cmake_print_variables(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(CopyRuntimeData
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/shaders
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/shaders
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/assets
)
add_dependencies(fabricatio CopyRuntimeData)
