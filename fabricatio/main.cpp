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
#include "producentis/Material.h"
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
        std::ranges::for_each(meshCatalog, [renderer, underwaterProgram] (auto namedMesh) 
                {
                    renderer->loadMesh(namedMesh.second);
                    auto material = namedMesh.second->getMaterial();
                    material->setShader(underwaterProgram);
                });
        
        ECSContainer ecs;
        ecs.registerType<Transform>();
        ecs.registerType<Mesh*>();
        ecs.registerType<Material>();
        ecs.registerType<TimeSpin>();

        Entity punkEntity = ecs.createEntity();
        Transform transform;
        transform.location = glm::vec3(-35.0f, 100.0f, -25.0f);
        ecs.addComponent<Transform>(punkEntity, std::move(transform));
        ecs.addComponent<Mesh*>(punkEntity, meshCatalog.getMesh("punk/punk.obj"));
        ecs.addComponent<TimeSpin>(punkEntity);

        Entity tijgerEntity = ecs.createEntity();
        transform.location = glm::vec3(35.0f, 100.0f, -25.0f);
        ecs.addComponent<Transform>(tijgerEntity, std::move(transform));
        ecs.addComponent<Mesh*>(tijgerEntity, meshCatalog.getMesh("tijger/tijger.obj"));
        ecs.addComponent<TimeSpin>(tijgerEntity);
        auto& timeSpin = ecs.getComponent<TimeSpin>(tijgerEntity);
        timeSpin.rotation.z = -1.0f;

        Entity girlEntity = ecs.createEntity();
        transform.location = glm::vec3(0.0f, 100.0f, -25.0f);
        transform.scale = glm::vec3(30.f, 30.f, 30.f);
        ecs.addComponent<Transform>(girlEntity, std::move(transform));
        ecs.addComponent<Mesh*>(girlEntity, meshCatalog.getMesh("katinka/Girl.obj"));

        std::set<Entity> entities = { punkEntity, tijgerEntity, girlEntity };

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
