#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "AppWindow.h"

void AppWindow::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

AppWindow::AppWindow(int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, "Conceptum", NULL, NULL);
    glfwSetWindowPos(m_window, (1920 - width) / 2, 32);
    if (m_window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(m_window, &AppWindow::framebuffer_size_callback);
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
