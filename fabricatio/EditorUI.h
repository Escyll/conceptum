#ifndef EDITORUI_H
#define EDITORUI_H

#include <entt/entt.hpp>
#include <ranges>

#include "glm/gtc/type_ptr.hpp"
#include "producentis/Renderer.h"

#include "PlayerControlComponent.h"
#include "RenderSystem.h"
#include "Transform.h"

void syncUIContext()
{
    ImGuiContext* context = imGuiCurrentContext();
    ImGui::SetCurrentContext(context);
}

void drawEditorUI(unsigned int renderTexture, const entt::registry& registry, RenderSystem& renderSystem)
{
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
}

#endif // EDITORUI_H
