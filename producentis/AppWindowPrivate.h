#ifndef APP_WINDOW_PRIVATE_H
#define APP_WINDOW_PRIVATE_H

#include <memory>

#include "producentis.h"
#include "AppWindow.h"
#include <GLFW/glfw3.h>

class producentis_API AppWindow
{
public:
    AppWindow(GLFWwindow *window) : m_window(window) {}
    ~AppWindow()
    {
        glfwTerminate();
        glfwDestroyWindow(m_window);
    }
    AppWindow(AppWindow &&) = default;
    GLFWwindow *operator*()
    {
        return m_window;
    }

private:
    GLFWwindow *m_window;
};

#endif