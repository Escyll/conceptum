#include <iostream>

#include "GlAppWindow.h"

AppWindow::AppWindow(Renderer* renderer, int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    m_window = glfwCreateWindow(width, height, "Fabricatio", NULL, NULL);
    glfwSetWindowPos(m_window, (1920 - width) / 2, 32);
    if (m_window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(0);
    renderer->initialize();
    renderer->resizeViewport(width, height);
    glfwSetWindowUserPointer(m_window, renderer);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
            {
                Renderer* renderer = (Renderer*) glfwGetWindowUserPointer(window);
                renderer->resizeViewport(width, height);
            });
}

AppWindow::~AppWindow()
{
    glfwTerminate();
}

bool AppWindow::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void AppWindow::processInput() const
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void AppWindow::swapBuffer() const
{
    glfwSwapBuffers(m_window);
}

void AppWindow::pollEvents() const
{
    glfwPollEvents();
}
