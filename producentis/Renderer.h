#ifndef PRODUCENTIS_RENDERER_H
#define PRODUCENTIS_RENDERER_H

#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "producentis.h"

#include "Mesh.h"
#include "AppWindow.h"
#include "Input.h"

producentis_API int createShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
producentis_API void useShaderProgram(int program);
producentis_API void loadMesh(Mesh *mesh);
producentis_API void drawMesh(Mesh *mesh, const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);
producentis_API void clearScreen();
producentis_API AppWindow *createWindow(int width, int height);
producentis_API void destroyWindow(AppWindow *window);
producentis_API bool shouldClose(AppWindow *window);
producentis_API void setShouldClose(AppWindow *window, bool shouldClose);
producentis_API Input processInput(AppWindow *window);
producentis_API void swapBuffer(AppWindow *window);
producentis_API void pollEvents(AppWindow *window);

#endif
