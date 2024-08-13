#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "producentis/Camera.h"
#include "producentis/Mesh.h"
#include "producentis/Renderer.h"

#include "RenderSystem.h"
#include "Transform.h"

RenderSystem::RenderSystem(entt::registry& registry)
    : System(registry)
{}

void RenderSystem::progress(float /*timeDelta*/)
{
    clearScreen(clearColor);
    auto cameraEntities = m_registry.view<Camera, Transform>();
    const auto& [cameraEntityCamera, cameraEntityTransform] = m_registry.get<Camera, Transform>(cameraEntities.front());

    auto view = m_registry.view<Mesh*, Transform>();
    for (const auto& [entity, mesh, transform] : view.each())
    {
        drawMesh(mesh, transform.transformation(), -cameraEntityTransform.transformation(), cameraEntityCamera.projection());
    }
}
