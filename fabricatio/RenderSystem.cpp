#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderSystem.h"
#include "Transform.h"
#include "producentis/Material.h"
#include "producentis/Mesh.h"
#include "producentis/Renderer.h"

RenderSystem::RenderSystem(ECSContainer &ecsContainer, Camera &camera)
    : m_ecsContainer(ecsContainer), m_camera(camera)
{
}

glm::mat4 getTransform(const Transform &transformComponent)
{
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, transformComponent.location);
    transform = glm::rotate(transform, transformComponent.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::rotate(transform, transformComponent.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, transformComponent.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::scale(transform, transformComponent.scale);
    return transform;
}

void RenderSystem::progress(float /*timeDelta*/, const std::set<Entity> &entities)
{
    clearScreen();
    for (auto entity : entities)
    {
        auto mesh = m_ecsContainer.getComponent<Mesh *>(entity);
        auto &modelTransform = m_ecsContainer.getComponent<Transform>(entity);

        drawMesh(mesh, shader, getTransform(modelTransform), m_camera.view(), m_camera.projection());
    }
}
