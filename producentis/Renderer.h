#ifndef PRODUCENTIS_RENDERER_H
#define PRODUCENTIS_RENDERER_H

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <string>

#include "fundamentum/Logger.h"
#include "producentis.h"

#include "AppWindow.h"
#include "Input.h"
#include "Lights.h"
#include "Mesh.h"

producentis_API int createShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
producentis_API void useShaderProgram(int program);
producentis_API void loadMesh(Mesh *mesh);
producentis_API void drawMesh(Mesh* mesh, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const Lights& lights);
producentis_API void clearScreen(const glm::vec4 &color);
producentis_API AppWindow *createWindow(int width, int height);
producentis_API void destroyWindow(AppWindow *window);
producentis_API bool shouldClose(AppWindow *window);
producentis_API void setShouldClose(AppWindow *window, bool shouldClose);
producentis_API Input processInput(AppWindow *window);
producentis_API void swapBuffer(AppWindow *window);
producentis_API void pollEvents(AppWindow *window);
producentis_API void enableCursor(AppWindow* window, bool showCursor);
producentis_API void newImGuiFrame();
producentis_API void imGuiRenderDrawData(ImDrawData *drawData);
producentis_API ImGuiContext *imGuiCurrentContext();
producentis_API void setLogContext(Log::LoggerContext* context);
producentis_API unsigned int createTexture(const glm::vec2& size);
producentis_API unsigned int createRenderBuffer(const glm::vec2& size);
producentis_API unsigned int createFramebuffer();
producentis_API void setFramebufferTextureAndBuffer(unsigned int framebuffer, unsigned int texture, unsigned int renderBuffer);
producentis_API void bindFramebuffer(unsigned int framebuffer);
producentis_API void unbindFramebuffer();

#endif
