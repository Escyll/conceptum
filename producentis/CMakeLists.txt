if(LINUX)
set(OpenGL_GL_PREFERENCE GLVND)
endif()
find_package(OpenGL REQUIRED)

add_library(producentis SHARED
    Lights.h )

target_sources(producentis PRIVATE
    "AppWindow.h"
    "AppWindowPrivate.h"
    "Material.h"
    "Material.cpp"
    "Mesh.cpp"
    "Mesh.h"   
    "Renderer.h"
    "Renderer.cpp"
    "ShaderCompiler.h"
    "ShaderCompiler.cpp"
    "ShaderProgram.h"
    "ShaderProgram.cpp"
    "Texture.h"
    "Texture.cpp"
    "Camera.h"
    "Camera.cpp"
    "KeyEvent.h"
    "KeyInput.h"
    "Input.h"
    "Lights.h"
)

target_include_directories(producentis PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(producentis PRIVATE fundamentum glfw OpenGL::GL glad glm::glm stb_image imgui)
