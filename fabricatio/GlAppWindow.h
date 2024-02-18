#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <GLFW/glfw3.h>

#include "producentis/Renderer.h"

class AppWindow
{
public:
    AppWindow(Renderer* renderer, int width, int height);
    ~AppWindow();

    bool shouldClose() const;
    void processInput() const;
    void swapBuffer() const;
    void pollEvents() const;

private:
    GLFWwindow *m_window = nullptr;
};

#endif
