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
#include "FastNoiseLite.h"

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
        std::cout << "UNDERWATER: " << underwaterProgram << std::endl;

        MeshCatalog meshCatalog;
        meshCatalog.loadMeshes("assets/meshes");
        std::ranges::for_each(meshCatalog, [renderer, underwaterProgram] (auto namedMesh) 
                {
                    renderer->loadMesh(namedMesh.second);
                    // auto material = namedMesh.second->getMaterial();
                    // material->setShader(underwaterProgram);
                });
        
        ECSContainer ecs;
        ecs.registerType<Transform>();
        ecs.registerType<Mesh*>();
        ecs.registerType<Material>();
        ecs.registerType<TimeSpin>();

        Entity punkEntity = ecs.createEntity();
        Transform transform;
        transform.location = glm::vec3(-1.0f, -0.33f, 0.0f);
        ecs.addComponent<Transform>(punkEntity, std::move(transform));
        ecs.addComponent<Mesh*>(punkEntity, meshCatalog.getMesh("punk/punk.obj"));

        Entity tijgerEntity = ecs.createEntity();
        transform = Transform();
        transform.location = glm::vec3(1.0f, -0.33f, 0.0f);
        transform.rotation = glm::vec3(0, 0, glm::radians(180.f));
        ecs.addComponent<Transform>(tijgerEntity, std::move(transform));
        ecs.addComponent<Mesh*>(tijgerEntity, meshCatalog.getMesh("tijger/tijger.obj"));

        Entity girlEntity = ecs.createEntity();
        transform = Transform();
        transform.location.y = 0.33;
        ecs.addComponent<Transform>(girlEntity, std::move(transform));
        ecs.addComponent<Mesh*>(girlEntity, meshCatalog.getMesh("katinka/Girl.obj"));

        std::set<Entity> entities = { punkEntity, tijgerEntity, girlEntity };

        FastNoiseLite noise;
        noise.SetFrequency(1.0/40.0);
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise.SetFractalType(FastNoiseLite::FractalType_FBm);
        for (int y = 0; y <= 100; y++)
        {
            for (int x = -100; x <= 100; x++)
            {
                float noiseVal = 0.5*noise.GetNoise((float)x, (float)y) + 0.5;
                for (int z = 0; z <= (int)floor(20*noiseVal); z++)
                {
                    Entity cubeEntity = ecs.createEntity();
                    transform = Transform();
                    transform.location = glm::vec3(x, y, z);
                    ecs.addComponent<Transform>(cubeEntity, std::move(transform));
                    ecs.addComponent<Mesh*>(cubeEntity, meshCatalog.getMesh("cube/cube.obj"));
                    entities.insert(cubeEntity);
                }
            }
        }



        SpinSystem spinSystem(ecs);
        RenderSystem renderSystem(ecs, renderer);
        renderSystem.shader = underwaterProgram;
        std::vector<System*> systems { &spinSystem, &renderSystem };
        // std::vector<System*> systems { &renderSystem };

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
