#include <FastNoiseLite.h>
#include <algorithm>
#include <cmath>
#include <entt/entt.hpp>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <iostream>
#include <numbers>
#include <ranges>
#include <stb_image.h>

#include "fabricatio/EditorUI.h"
#include "producentis/Camera.h"
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
#include "ScalarGrid.h"
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
        entt::registry registry;

        auto player = registry.create();
        registry.emplace<PlayerControlComponent>(player);
        registry.emplace<Camera>(player);
        registry.emplace<Transform>(player);

        FastNoiseLite noise;
        noise.SetSeed(200);
        noise.SetFractalOctaves(3);
        noise.SetFrequency(1.0 / 20.0);
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise.SetFractalType(FastNoiseLite::FractalType_FBm);

        constexpr auto rangeX = std::ranges::iota_view{0, 20};
        constexpr auto rangeY = std::ranges::iota_view{0, 20};
        constexpr auto rangeZ = std::ranges::iota_view{0, 10};
        auto rangeZReversed = rangeZ | std::views::reverse;

        ScalarGrid noiseGrid(glm::ivec3{rangeX.size(), rangeY.size(), rangeZ.size()});

        for (int x : rangeX)
        {
            for (int y : rangeY)
            {
                auto noise2D = 0.5f + 0.5f * noise.GetNoise((float) x, (float) y);
                auto surfaceNoise = 5.f * noise2D;
                auto cosX = 0.5f * (cos(x * std::numbers::pi * 2.f / 200.f) + 1.f);
                auto cosY = 0.5f * (cos(y * std::numbers::pi * 2.f / 200.f) + 1.f);
                auto surfaceHeight = 30.f * std::max(cosX, cosY) + 30.f + surfaceNoise;
                auto caveCeil = 40.f - surfaceNoise;

                for (auto z : rangeZ)
                {
                    auto caveNoise = 0.5f + 0.5f * noise.GetNoise(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                    if (z < (int) std::floor(surfaceHeight) && z < caveCeil)
                    {
                        noiseGrid.setScalar(glm::ivec3{x, y, z}, caveNoise);
                    } else if (z == (int) std::floor(surfaceHeight) && z < caveCeil && caveNoise > 0.65f)
                    {
                        noiseGrid.setScalar(glm::ivec3{x, y, z}, caveNoise);
                    } else
                    {
                        if (noiseGrid.scalar(glm::ivec3{x, y, z - 1}) > 0.67f || noiseGrid.scalar(glm::ivec3{x, y, z - 2}) > 0.67f)
                            noiseGrid.setScalar(glm::ivec3{x, y, z}, 0.66f);
                        else
                            noiseGrid.setScalar(glm::ivec3{x, y, z}, 0.65f + 0.1 * (z - surfaceHeight));
                    }
                }
            }
        }

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* textureData = stbi_load("assets/textures/Grass.jpg", &width, &height, &nrChannels, 0);
        auto grass = std::make_unique<Texture>(textureData, width, height, nrChannels);
        textureData = stbi_load("assets/textures/Rock.jpg", &width, &height, &nrChannels, 0);
        auto rock = std::make_unique<Texture>(textureData, width, height, nrChannels);

        Mesh* terrain = MarchingCubes::march(noiseGrid, 0.65);
        auto& materials = terrain->getMaterials();
        for (auto& subMesh : terrain->getSubMeshes())
        {
            materials[subMesh.materialName]->setShader(terrainProgram);
            materials[subMesh.materialName]->setTexture("grass", std::move(grass));
            materials[subMesh.materialName]->setTexture("rock", std::move(rock));
        }

        loadMesh(terrain);
        auto terrainEntity = registry.create();
        registry.emplace<Transform>(terrainEntity);
        registry.patch<Transform>(terrainEntity, [&rangeX, &rangeY](Transform& transform) {
            // transform.location = glm::vec3(-0.5 * rangeX.size(), -0.5 * rangeY.size(), 0);
            transform.location = glm::vec3(0, 10, 0);
        });
        registry.emplace<Mesh*>(terrainEntity, terrain);

        InputSystem inputSystem;
        RenderSystem renderSystem(registry);
        PlayerControllerSystem playerControllerSystem(registry, inputSystem);
        std::vector<System*> systems{&playerControllerSystem, &renderSystem};

        auto renderTexture = createTexture({1024, 768});
        auto frameBuffer = createFramebuffer();
        setFramebufferTexture(frameBuffer, renderTexture);

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
