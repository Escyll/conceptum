project(App)

add_executable(RayTracer)

target_sources(RayTracer
    PRIVATE
      AppWindow.cpp
      AppWindow.h
      Clock.cpp
      Clock.h
      main.cpp
      Material.cpp
      Material.h
      Mesh.cpp
      Mesh.h
      ObjectRenderer.cpp
      ObjectRenderer.h
      Object.cpp
      Object.h
      Renderer.cpp
      Renderer.h
      ShaderCompiler.cpp
      ShaderCompiler.h
      ShaderProgram.cpp
      ShaderProgram.h
)

target_link_libraries(RayTracer PRIVATE glfw OpenGL::GL glad glm::glm)

add_custom_target(CopyShaders
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/shaders
)
add_custom_target(CopyAssets
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/$(Configuration)/assets
)
add_dependencies(RayTracer CopyShaders)
add_dependencies(RayTracer CopyAssets)