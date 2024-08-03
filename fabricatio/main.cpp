#include <iostream>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <set>
#include <algorithm>
#include <ranges>
#include <array>
#include <numbers>

#include "producentis/Renderer.h"
#include "producentis/Input.h"
#include "producentis/Mesh.h"
#include "producentis/Material.h"
#include "producentis/Camera.h"

#include "Clock.h"
#include "RenderSystem.h"
#include "ECSContainer.h"
#include "Transform.h"
#include "SpinSystem.h"
#include "IO.h"
#include "MarchingCubes.h"
#include "MeshCatalog.h"
#include "FastNoiseLite.h"
#include "ScalarGrid.h"
#include "InputSystem.h"
#include <stb_image.h>

int main()
{
    try
    {
        std::cout << "Starting" << std::endl;
        std::cout << "Current path is " << std::filesystem::current_path() << '\n';
        AppWindow *appWindow = createWindow(1920, 1080);

        auto standardVert = IO::readFile("shaders/standard.vert");
        auto diffuseFrag = IO::readFile("shaders/underwater.frag");
        auto terrainFrag = IO::readFile("shaders/terrain.frag");
        auto diffuseProgram = createShaderProgram(standardVert, diffuseFrag);
        auto terrainProgram = createShaderProgram(standardVert, terrainFrag);
        useShaderProgram(terrainProgram);

        MeshCatalog meshCatalog;
        ECSContainer ecs;
        ecs.registerType<Transform>();
        ecs.registerType<Mesh *>();
        ecs.registerType<Material>();
        ecs.registerType<TimeSpin>();
        std::set<Entity> entities;

        Transform transform;

        bool drawCatsAndGirl = false;
        if (drawCatsAndGirl)
        {
            meshCatalog.loadMeshes("assets/meshes");
            std::ranges::for_each(meshCatalog, [diffuseProgram](auto namedMesh)
                                  {
                                      loadMesh(namedMesh.second);
                                      auto &materials = namedMesh.second->getMaterials();
                                      for (auto &subMesh : namedMesh.second->getSubMeshes())
                                      {
                                          materials[subMesh.materialName]->setShader(diffuseProgram);
                                      } });

            Entity punkEntity = ecs.createEntity();
            transform.location = glm::vec3(-1.0f, -0.33f, 0.0f);
            ecs.addComponent<Transform>(punkEntity, std::move(transform));
            ecs.addComponent<Mesh *>(punkEntity, meshCatalog.getMesh("punk/punk.obj"));

            Entity tijgerEntity = ecs.createEntity();
            transform = Transform();
            transform.location = glm::vec3(1.0f, -0.33f, 0.0f);
            transform.rotation = glm::vec3(0, 0, glm::radians(180.f));
            ecs.addComponent<Transform>(tijgerEntity, std::move(transform));
            ecs.addComponent<Mesh *>(tijgerEntity, meshCatalog.getMesh("tijger/tijger.obj"));

            Entity girlEntity = ecs.createEntity();
            transform = Transform();
            transform.location.y = 0.33;
            ecs.addComponent<Transform>(girlEntity, std::move(transform));
            ecs.addComponent<Mesh *>(girlEntity, meshCatalog.getMesh("katinka/Girl.obj"));

            entities = {punkEntity, tijgerEntity, girlEntity};
        }

        FastNoiseLite noise;
        noise.SetSeed(200);
        noise.SetFractalOctaves(3);
        noise.SetFrequency(1.0 / 20.0);
        noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise.SetFractalType(FastNoiseLite::FractalType_FBm);

        constexpr auto rangeX = std::ranges::iota_view{0, 200};
        constexpr auto rangeY = std::ranges::iota_view{0, 200};
        constexpr auto rangeZ = std::ranges::iota_view{0, 100};
        auto rangeZReversed = rangeZ | std::views::reverse;

        ScalarGrid noiseGrid(glm::ivec3{rangeX.size(), rangeY.size(), rangeZ.size()});

        for (int x : rangeX)
        {
            for (int y : rangeY)
            {
                auto noise2D = 0.5f + 0.5f * noise.GetNoise((float)x, (float)y);
                auto surfaceNoise = 5.f * noise2D;
                auto cosX = 0.5f * (cos(x * std::numbers::pi * 2.f / 200.f) + 1.f);
                auto cosY = 0.5f * (cos(y * std::numbers::pi * 2.f / 200.f) + 1.f);
                auto surfaceHeight = 30.f * std::max(cosX, cosY) + 30.f + surfaceNoise;
                auto caveCeil = 40.f - surfaceNoise;

                for (auto z : rangeZ)
                {
                    auto caveNoise = 0.5f + 0.5f * noise.GetNoise(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
                    if (z < (int)std::floor(surfaceHeight) && z < caveCeil)
                    {
                        noiseGrid.setScalar(glm::ivec3{x, y, z}, caveNoise);
                    }
                    else if (z == (int)std::floor(surfaceHeight) && z < caveCeil && caveNoise > 0.65f)
                    {
                        noiseGrid.setScalar(glm::ivec3{x, y, z}, caveNoise);
                    }
                    else
                    {
                        if (noiseGrid.scalar(glm::ivec3{x, y, z - 1}) > 0.67f || noiseGrid.scalar(glm::ivec3{x, y, z - 2}) > 0.67f)
                            noiseGrid.setScalar(glm::ivec3{x, y, z}, 0.66f);
                        else
                            noiseGrid.setScalar(glm::ivec3{x, y, z}, 0.65f + 0.1 * (z - surfaceHeight));
                    }
                }
            }
        }

        Camera camera;
        camera.setPosition(glm::vec3{0.0, 0.0, 0.0});

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *textureData = stbi_load("assets/textures/Grass.jpg", &width, &height, &nrChannels, 0);
        auto grass = std::make_unique<Texture>(textureData, width, height, nrChannels);
        textureData = stbi_load("assets/textures/Rock.jpg", &width, &height, &nrChannels, 0);
        auto rock = std::make_unique<Texture>(textureData, width, height, nrChannels);

        Mesh *terrain = MarchingCubes::March(noiseGrid, 0.65);
        auto &materials = terrain->getMaterials();
        for (auto &subMesh : terrain->getSubMeshes())
        {
            materials[subMesh.materialName]->setShader(terrainProgram);
            materials[subMesh.materialName]->setTexture("grass", std::move(grass));
            materials[subMesh.materialName]->setTexture("rock", std::move(rock));
        }

        loadMesh(terrain);
        Entity terrainEntity = ecs.createEntity();
        transform = Transform();
        transform.location = {-0.5 * rangeX.size(), -0.5 * rangeY.size(), 0};
        ecs.addComponent<Transform>(terrainEntity, std::move(transform));
        ecs.addComponent<Mesh *>(terrainEntity, std::move(terrain));
        entities.insert(terrainEntity);

        SpinSystem spinSystem(ecs);
        RenderSystem renderSystem(ecs, camera);
        // std::vector<System *> systems{&spinSystem, &renderSystem};
        std::vector<System *> systems{&renderSystem};

        Clock clock;
        const float fpsLimit = 1.0f / 240.0f;
        float lastUpdateTime = 0;
        float lastFrameTime = 0;
        glm::vec3 cameraMotion = {0, 0, 0};
        float speedModifier = 1.0;
        constexpr float cameraBaseSpeed = 6;
        InputSystem inputSystem;
        glm::fquat rotation;
        while (!shouldClose(appWindow))
        {
            float elapsedSeconds = clock.elapsedSeconds();
            float timeSinceLastUpdate = elapsedSeconds - lastUpdateTime;
            pollEvents(appWindow);
            inputSystem.processInput(processInput(appWindow));
            auto globalInput = inputSystem.globalInput();
            for (auto input : globalInput)
            {
                switch (input.action)
                {
                case GlobalInput::ESCAPE:
                    setShouldClose(appWindow, true);
                }
            }
            auto movementInput = inputSystem.movementInput();
            for (auto input : movementInput)
            {
                switch (input.movement)
                {
                case MovementInput::FORWARD:
                    cameraMotion.y = input.action == MovementInput::START ? 1 : 0;
                    break;
                case MovementInput::BACKWARD:
                    cameraMotion.y = input.action == MovementInput::START ? -1 : 0;
                    break;
                case MovementInput::LEFT:
                    cameraMotion.x = input.action == MovementInput::START ? -1 : 0;
                    break;
                case MovementInput::RIGHT:
                    cameraMotion.x = input.action == MovementInput::START ? 1 : 0;
                    break;
                case MovementInput::UP:
                    cameraMotion.z = input.action == MovementInput::START ? 1 : 0;
                    break;
                case MovementInput::DOWN:
                    cameraMotion.z = input.action == MovementInput::START ? -1 : 0;
                    break;
                case MovementInput::SPRINT:
                    speedModifier = input.action == MovementInput::START ? 2.0 : 1.0;
                    break;
                }
            }
            float cameraSpeed = cameraBaseSpeed * speedModifier;
            camera.moveRight(cameraSpeed * timeSinceLastUpdate * cameraMotion.x);
            camera.moveForward(cameraSpeed * timeSinceLastUpdate * cameraMotion.y);
            camera.moveUp(cameraSpeed * timeSinceLastUpdate * cameraMotion.z);
            auto axisInput = inputSystem.axisInput();
            camera.pitch(axisInput.second);
            camera.yaw(axisInput.first);

            if (elapsedSeconds - lastFrameTime >= fpsLimit)
            {
                // std::cout << elapsedSeconds - lastFrameTime << std::endl;
                float timeDelta = clock.deltaSeconds();
                for (auto system : systems)
                {
                    system->progress(timeDelta, entities);
                }
                swapBuffer(appWindow);

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
