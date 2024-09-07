include(CMakePrintHelpers)

add_executable(fabricatio
    EditorUI.h
    TerrainGenerator.h TerrainGenerator.cpp
    Serializer.h Serializer.cpp)

set(sources
    "Clock.cpp"
    "Clock.h"
    "EditorUI.h"
    "IO.cpp"
    "IO.h"
    "InputSystem.cpp"
    "InputSystem.h"
    "MarchingCubes.cpp"
    "MarchingCubes.h"
    "MeshCatalog.cpp"
    "MeshCatalog.h"
    "PlayerControlComponent.h"
    "PlayerControllerSystem.cpp"
    "PlayerControllerSystem.h"
    "RenderSystem.cpp"
    "RenderSystem.h"
    "ScalarGrid.cpp"
    "ScalarGrid.h"
    "Serializer.cpp"
    "Serializer.h"
    "System.h"
    "TerrainGenerator.cpp"
    "TerrainGenerator.h"
    "Transform.cpp"
    "Transform.h"
    "main.cpp"
)

target_sources(fabricatio
  PRIVATE
    ${sources}
)

target_link_libraries(fabricatio PRIVATE glm::glm stb_image FastNoiseLite producentis fundamentum imgui EnTT::EnTT)

cmake_print_variables(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
add_custom_target(CopyRuntimeData
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/shaders
  COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/assets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/shaders
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/assets
)
add_dependencies(fabricatio CopyRuntimeData)
