add_executable(fabricatio)

target_sources(fabricatio
    PRIVATE
      "AppWindow.cpp"
      "AppWindow.h"
      "Clock.cpp"
      "Clock.h"
      "ECSContainer.h"
      "main.cpp"
      "Material.cpp"
      "Material.h"
      "Mesh.cpp"
      "Mesh.h"   
      "Renderer.cpp"
      "Renderer.h"
      "ShaderCompiler.cpp"
      "ShaderCompiler.h"
      "ShaderProgram.cpp"
      "ShaderProgram.h"
      "SpinSystem.h"
      "SpinSystem.cpp"
      "Transform.h"
      "IO.h"
      "IO.cpp"
 "RenderSystem.h" "RenderSystem.cpp")

target_link_libraries(fabricatio PRIVATE glfw OpenGL::GL glad glm::glm stb_image)

add_custom_target(CopyShaders
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/${Configuration}/shaders
)
add_custom_target(CopyAssets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/${Configuration}/assets
)
add_dependencies(fabricatio CopyShaders)
add_dependencies(fabricatio CopyAssets)