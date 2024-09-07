#include <FastNoiseLite.h>
#include <entt/entt.hpp>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <iostream>
#include <stb_image.h>

#include "fabricatio/EditorUI.h"
#include "producentis/Camera.h"
#include "producentis/Lights.h"
#include "producentis/Material.h"
#include "producentis/Mesh.h"
#include "producentis/Renderer.h"
#include <fundamentum/Logger.h>

#include "Clock.h"
#include "IO.h"
#include "InputSystem.h"
#include "MarchingCubes.h"
#include "MeshCatalog.h"
#include "PlayerControlComponent.h"
#include "PlayerControllerSystem.h"
#include "RenderSystem.h"
#include "Serializer.h"
#include "TerrainGenerator.h"
#include "Transform.h"

int main()
{
    auto logContext = Log::createContext();
    setLogContext(logContext);
    Log::log() << "Starting" << Log::end;
    Log::log() << "Current path is " << std::filesystem::current_path().string() << Log::end;
    try
    {
        AppWindow* appWindow = createWindow(1920, 1080);

        auto standardVert = IO::readFile("shaders/standard.vert");
        // auto diffuseFrag = IO::readFile("shaders/diffuse.frag");
        auto terrainFrag = IO::readFile("shaders/terrain.frag");
        // auto diffuseProgram = createShaderProgram(standardVert, diffuseFrag);
        auto terrainProgram = createShaderProgram(standardVert, terrainFrag);
        useShaderProgram(terrainProgram);

        MeshCatalog meshCatalog;

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* textureData = stbi_load("assets/textures/Grass.jpg", &width, &height, &nrChannels, 0);
        auto grass = std::make_unique<Texture>(textureData, width, height, nrChannels);
        textureData = stbi_load("assets/textures/Rock.jpg", &width, &height, &nrChannels, 0);
        auto rock = std::make_unique<Texture>(textureData, width, height, nrChannels);

        TerrainGenerator terrainGenerator(200);
        auto terrain = terrainGenerator.generateTerrainMesh();
        auto& materials = terrain->materialsMap;
        for (auto& subMesh : terrain->subMeshes)
        {
            materials[subMesh.materialName]->setShader(terrainProgram);
            materials[subMesh.materialName]->setTexture("grass", std::move(grass));
            materials[subMesh.materialName]->setTexture("rock", std::move(rock));
        }

        loadMesh(terrain);

        entt::registry registry;

        // Player
        auto player = registry.create();
        registry.emplace<PlayerControlComponent>(player);
        registry.emplace<Camera>(player);
        registry.emplace<Transform>(player);

        // Terrain
        auto terrainEntity = registry.create();
        auto& terrainTrans = registry.emplace<Transform>(terrainEntity);
        terrainTrans.location = glm::vec3(0, 0, 0);
        registry.emplace<Mesh*>(terrainEntity, terrain);

        // Light
        auto light = registry.create();
        auto& lightTrans = registry.emplace<Transform>(light);
        lightTrans.location = glm::vec3(1, 1, 1);
        registry.emplace<PointLight>(light, 1.f, glm::vec3{1, 1, 1});

        InputSystem inputSystem;
        RenderSystem renderSystem(registry);
        PlayerControllerSystem playerControllerSystem(registry, inputSystem);
        std::vector<System*> systems{&playerControllerSystem, &renderSystem};

        auto frameBuffer = createFramebuffer();
        auto renderTexture = createTexture({1024, 768});
        auto renderBuffer = createRenderBuffer({1024, 768});
        setFramebufferTextureAndBuffer(frameBuffer, renderTexture, renderBuffer);

        Clock clock;
        const float fpsLimit = 1.0f / 60.0f;
        float lastFrameTime = 0;

        syncUIContext();

        bool inMenuMode = false;
        while (!shouldClose(appWindow))
        {
            float elapsedSeconds = clock.elapsedSeconds();
            if (elapsedSeconds - lastFrameTime >= fpsLimit)
            {
                float timeDelta = elapsedSeconds - lastFrameTime;
                lastFrameTime = elapsedSeconds;

                pollEvents(appWindow);
                inputSystem.processInput(processInput(appWindow));
                auto globalInput = inputSystem.globalInput();
                for (auto input : globalInput)
                {
                    switch (input.action)
                    {
                    case GlobalInput::ESCAPE:
                        setShouldClose(appWindow, true);
                        break;
                    case GlobalInput::TOGGLE_MENU:
                        inMenuMode = !inMenuMode;
                        enableCursor(appWindow, inMenuMode);
                        break;
                    case GlobalInput::SAVE_SCENE: {
                        Log::log() << "Saving scene..." << Log::end;
                        Serializer::serialize(registry);
                        Log::log() << "Saving scene... Done" << Log::end;
                        break;
                    }
                    case GlobalInput::LOAD_SCENE: {
                        Log::log() << "Load scene..." << Log::end;
                        break;
                    }
                    default:
                        break;
                    }
                }

                bindFramebuffer(frameBuffer);
                for (auto system : systems)
                {
                    system->progress(timeDelta);
                }
                unbindFramebuffer();
                clearScreen({});
                drawEditorUI(renderTexture, registry, renderSystem);

                swapBuffer(appWindow);
            }
        }
    } catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    Log::destroyContext(logContext);
    return 0;
}
