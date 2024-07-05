#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderSystem.h"
#include "Transform.h"
#include "producentis/Material.h"

RenderSystem::RenderSystem(ECSContainer &ecsContainer, Renderer* renderer)
    : m_ecsContainer(ecsContainer)
    , m_renderer(renderer)
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

void RenderSystem::progress(float /*timeDelta*/, const std::set<Entity>& entities)
{
    m_renderer->clearScreen();
    for (auto entity : entities)
    {
        auto mesh = m_ecsContainer.getComponent<Mesh*>(entity);
        auto& transform = m_ecsContainer.getComponent<Transform>(entity);
        
        Transform viewTransform;
        viewTransform.location = glm::vec3(0.0, 20.0, -20.0f);
        viewTransform.rotation = glm::vec3(glm::radians(20.f), 0, 0);
        auto projection = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        projection = glm::perspective(glm::radians(60.0f), 1920.0f / 1080.0f, 0.1f, 200.0f) * projection;
        m_renderer->drawMesh(mesh, shader, getTransform(transform), getTransform(viewTransform), projection);
    }
}

