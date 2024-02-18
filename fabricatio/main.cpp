#include <iostream>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <set>
#include <algorithm>

#include "GlAppWindow.h"
#include "Clock.h"
#include "RenderSystem.h"
#include "ECSContainer.h"
#include "Transform.h"
#include "producentis/Mesh.h"
#include "Material.h"
#include "SpinSystem.h"
#include "IO.h"
#include "NixRendererLoader.h"
#include "MeshCatalog.h"

int main()
{
    try
    {
        std::cout << "Starting" << std::endl;
        auto renderer = createRenderer();
        std::cout << "Renderer created" << std::endl;

        std::cout << "Current path is " << std::filesystem::current_path() << '\n';
        AppWindow appWindow(renderer, 1000, 1000);

        auto standardVert = IO::readFile("shaders/standard.vert");
        auto standardFrag = IO::readFile("shaders/standard.frag");
        auto underwaterFrag = IO::readFile("shaders/underwater.frag");
        auto standardProgram = renderer->createShaderProgram(standardVert, standardFrag);
        auto underwaterProgram = renderer->createShaderProgram(standardVert, underwaterFrag);

        MeshCatalog meshCatalog;
        meshCatalog.loadMeshes("assets/meshes");
        std::ranges::for_each(meshCatalog, [renderer] (auto namedMesh) { renderer->loadMesh(namedMesh.second); });
        
        ECSContainer ecs;
        ecs.registerType<Transform>();
        ecs.registerType<Mesh*>();
        ecs.registerType<Material>();
        ecs.registerType<TimeSpin>();

        Entity containerEntity = ecs.createEntity();
        Transform transform;
        transform.location = glm::vec3(0.0f, 100.0f, -25.0f);
        ecs.addComponent<Transform>(containerEntity, std::move(transform));
        ecs.addComponent<Mesh*>(containerEntity, meshCatalog.getMesh("cat/cat.obj"));
        ecs.addComponent<Material>(containerEntity, Material(underwaterProgram, "assets/textures/containers.jpg"));
        // ecs.AddComponent<TimeSpin>(containerEntity);
        
        std::set<Entity> entities = { containerEntity };

        SpinSystem spinSystem(ecs);
        RenderSystem renderSystem(ecs, renderer);
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
                    //renderSystem.UseShaderProgram(standardProgram);
                    system->progress(timeDelta, entities);
                }
                appWindow.swapBuffer();

                lastFrameTime = elapsedSeconds;
            }
            lastUpdateTime = elapsedSeconds;
        }
        destroyRenderer(renderer);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
