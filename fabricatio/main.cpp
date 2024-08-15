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
        ImGuiContext* context = imGuiCurrentContext();
        ImGui::SetCurrentContext(context);
        bool inMenuMode = false;
        while (!shouldClose(appWindow))
        {
            float elapsedSeconds = clock.elapsedSeconds();
            if (elapsedSeconds - lastFrameTime >= fpsLimit)
            {
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

                float timeDelta = clock.deltaSeconds();

                bindFramebuffer(frameBuffer);
                for (auto system : systems)
                {
                    system->progress(timeDelta);
                }
                unbindFramebuffer();

                clearScreen({});

                newImGuiFrame();
                ImGui::NewFrame();

                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                    // and append into it.
                auto view = registry.view<PlayerControlComponent, Transform>();
                for (const auto& [pcc, t] : view.each())
                {
                    ImGui::Text("Location: %f %f %f", t.location.x, t.location.y, t.location.z);
                    ImGui::Text("Rotation: %f %f %f", t.rotation.x, t.rotation.y, t.rotation.z);
                }
                ImGui::Text("This is some useful text."); // Display some text (you can
                    // use a format strings too)

                ImGui::SliderFloat("float", &f, 0.0f,
                                   1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color",
                                  glm::value_ptr(renderSystem.clearColor)); // Edit 3 floats representing a color

                if (ImGui::Button("Button")) // Buttons return true when clicked (most
                    // widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                // ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
                // io.Framerate, io.Framerate);
                ImGui::End();

                ImGui::Begin("Hello2!");
                static bool autoScroll = true;
                ImGui::Checkbox("Autoscroll", &autoScroll);
                ImGui::BeginChild("LogScroll");
                for (const auto& line : std::ranges::reverse_view(Log::lastNLogLines(1000)))
                {
                    ImGui::Text("%s", line.c_str());
                }
                if (autoScroll)
                    ImGui::SetScrollHereY(1.0);
                ImGui::EndChild();
                ImGui::End();

                ImGui::Begin("ViewPort");
                ImTextureID texture_id = reinterpret_cast<ImTextureID>(renderTexture);
                ImGui::Image(texture_id, ImVec2(1024, 768), {0, 1}, {1, 0});
                ImGui::End();

                ImGui::Render();
                imGuiRenderDrawData(ImGui::GetDrawData());
                swapBuffer(appWindow);

                lastFrameTime = elapsedSeconds;
            }
        }
    } catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    Log::destroyContext(logContext);
    return 0;
}
