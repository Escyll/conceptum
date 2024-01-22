#ifndef APPWINDOW_H
#define APPWINDOW_H

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AppWindow
{
public:
    AppWindow(int width, int height);
    ~AppWindow();

    bool shouldClose() const;
    void processInput() const;
    void swapBuffer() const;
    void pollEvents() const;

private:
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    GLFWwindow *m_window = nullptr;
};

#endif