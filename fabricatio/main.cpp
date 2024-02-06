#include <iostream>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <set>

#include "AppWindow.h"
#include "ShaderCompiler.h"
#include "ShaderProgram.h"
#include "Clock.h"
#include "Renderer.h"
#include "ECSContainer.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "SpinSystem.h"
#include "IO.h"

int main()
{
    try
    {
        ECSContainer ecs;
        RenderSystem renderSystem(ecs);
        std::cout << "Current path is " << std::filesystem::current_path() << '\n';
        AppWindow appWindow(1000, 1000);

        auto standardVert = IO::readFile("shaders/standard.vert");
        auto standardFrag = IO::readFile("shaders/standard.frag");
        auto underwaterFrag = IO::readFile("shaders/underwater.frag");
        auto standardProgram = renderSystem.CreateShaderProgram(standardVert, standardFrag);
        auto underwaterProgram = renderSystem.CreateShaderProgram(standardVert, underwaterFrag);

        ecs.Register<Transform>();
        ecs.Register<Mesh>();
        ecs.Register<Material>();
        ecs.Register<TimeSpin>();

        Entity containerEntity = ecs.CreateEntity();
        Transform transform;
        transform.location = glm::vec3(-0.5f * std::sqrt(2.f) * 6.0f, 0.0f, 0.5f * std::sqrt(2.f) * 6.0f);
        transform.rotation = glm::vec3(0.0f, glm::radians(-45.f), 0.0f);
        ecs.AddComponent<Transform>(containerEntity, std::move(transform));
        ecs.AddComponent<Mesh>(containerEntity, Mesh({ -0.9f, -0.9f, 0.0f, 0.9f, -0.9f, 0.0f, -0.9f, 0.9f, 0.0f, 0.9f, 0.9f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
            { 0, 1, 2, 1, 3, 2 }));
        ecs.AddComponent<Material>(containerEntity, Material(underwaterProgram, "assets/containers.jpg"));
        ecs.AddComponent<TimeSpin>(containerEntity);
        
        std::set<Entity> entities = { containerEntity };
        SpinSystem spinSystem(ecs);
        
        renderSystem.loadBuffers(entities);
        std::vector<System*> systems { &spinSystem, &renderSystem };

        Clock clock;
        const float fpsLimit = 1.0f/240.0f;
        float lastUpdateTime = 0;
        float lastFrameTime = 0;
        while (!appWindow.shouldClose())
        {
            float elapsedSeconds = clock.elapsedSeconds();
            float timeSinceLastUpdate = elapsedSeconds - lastUpdateTime;
            appWindow.pollEvents();
            appWindow.processInput();

            if (elapsedSeconds - lastFrameTime >= fpsLimit)
            {
                float timeDelta = clock.deltaSeconds();
                for (auto system : systems)
                {
                    renderSystem.UseShaderProgram(standardProgram);
                    system->Progress(timeDelta, entities);
                }
                appWindow.swapBuffer();

                lastFrameTime = elapsedSeconds;
            }
            lastUpdateTime = elapsedSeconds;
        }
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}