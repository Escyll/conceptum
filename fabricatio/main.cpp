#include <iostream>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
        
        struct TimeSpin
        {
            glm::vec3 rotation{ 1.0f, 0.0f, 0.0f };
        };
        ECSContainer ecs;
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
        ecs.AddComponent<Material>(containerEntity, Material(&underwaterProgram, "assets/containers.jpg"));
        ecs.AddComponent<TimeSpin>(containerEntity);

        class SpinSystem : public System
        {
        public:
            SpinSystem(ECSContainer& ecs, const std::set<Entity>& entities) : m_ecs(ecs), m_entities(entities) {}
            void Progress(float timeDelta) override
            {
                for (auto entity : m_entities)
                {
                    auto& timeSpin = m_ecs.GetComponent<TimeSpin>(entity);
                    auto& transform = m_ecs.GetComponent<Transform>(entity);
                    transform.rotation += timeDelta * timeSpin.rotation;
                }
            }
        private:
            ECSContainer& m_ecs;
            std::set<Entity> m_entities;
        };
        std::set<Entity> entities = { containerEntity };
        SpinSystem spinSystem(ecs, entities);
        RenderSystem renderSystem(ecs, entities);
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
                    system->Progress(timeDelta);
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