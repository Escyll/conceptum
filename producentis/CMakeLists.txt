add_library(producentis SHARED)

target_sources(producentis
	PRIVATE
    "Mesh.cpp"
    "Mesh.h"   
	"Renderer.h"
	"Renderer.cpp"
	"ShaderCompiler.h"
	"ShaderCompiler.cpp"
	"ShaderProgram.h"
	"ShaderProgram.cpp"
)

target_include_directories(producentis PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(producentis PRIVATE glfw OpenGL::GL glad glm::glm stb_image)
