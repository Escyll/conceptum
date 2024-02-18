#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderSystem.h"
#include "Transform.h"
#include "Material.h"

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
        auto& material = m_ecsContainer.getComponent<Material>(entity);
        auto& transform = m_ecsContainer.getComponent<Transform>(entity);
        auto shader = material.getShader();

        auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        auto projection = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f) * projection;
        m_renderer->drawMesh(mesh, shader, getTransform(transform), view, projection);
    }
}

