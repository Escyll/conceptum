#include <iostream>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AppWindow.h"
#include "ShaderCompiler.h"
#include "ShaderProgram.h"
#include "Clock.h"

#include "Object.h"
#include "Renderer.h"

int main()
{
    try
    {
        std::cout << "Current path is " << std::filesystem::current_path() << '\n';
        AppWindow appWindow(1000, 1000);

        int standardVertexShader = ShaderCompiler::compileShader("shaders/standard.vert", ShaderCompiler::ShaderType::Vertex);
        int standardFragmentShader = ShaderCompiler::compileShader("shaders/standard.frag", ShaderCompiler::ShaderType::Fragment);
        int underwaterFragmentShader = ShaderCompiler::compileShader("shaders/underwater.frag", ShaderCompiler::ShaderType::Fragment);

        auto standardProgram = ShaderProgram({standardVertexShader, standardFragmentShader});
        auto underwaterProgram = ShaderProgram({standardVertexShader, underwaterFragmentShader});

        glDeleteShader(standardVertexShader);
        glDeleteShader(standardFragmentShader);
        glDeleteShader(underwaterFragmentShader);

        Mesh containerMesh({-0.9f, -0.9f, 0.0f, 0.9f, -0.9f, 0.0f, -0.9f, 0.9f, 0.0f, 0.9f, 0.9f, 0.0f},
                           {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
                           {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f},
                           {0, 1, 2, 1, 3, 2});
        Material material(&underwaterProgram, "assets/containers.jpg");
        ObjectRenderer objectRenderer(&material);
        Object container(&objectRenderer, containerMesh);
        container.setLocation(glm::vec3(-0.5f * std::sqrt(2.f) * 6.0f, 0.0f, 0.5f * std::sqrt(2.f) * 6.0f));
        std::vector<Object *> objects{&container};
        Renderer renderer;

        renderer.loadBuffers(objects);

        Clock clock;
        while (!appWindow.shouldClose())
        {
            appWindow.processInput();

            float time = clock.elapsedSeconds();
            container.setRotation(glm::vec3(time, glm::radians(-45.f), 0.0f));

            renderer.setTime(time);
            renderer.render(objects);

            appWindow.swapBuffer();
            appWindow.pollEvents();
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}