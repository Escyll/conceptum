#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

RenderSystem::RenderSystem(ECSContainer &ecsContainer, const std::set<Entity>& entities)
    : m_ecsContainer(ecsContainer)
    , m_entities(entities)
{
    loadBuffers();
}

void RenderSystem::loadBuffers() const
{
    for (auto entity : m_entities)
    {
        auto& mesh = m_ecsContainer.GetComponent<Mesh>(entity);
        auto& material = m_ecsContainer.GetComponent<Material>(entity);

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        unsigned int textureId;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        auto vertexBuffer = mesh.getVertexBuffer();
        auto indices = mesh.getIndices();
        glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), &vertexBuffer[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        mesh.setVao(VAO);

        auto texture = material.getTexture();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, material.getTextureWidth(), material.getTextureHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
        glGenerateMipmap(GL_TEXTURE_2D);

        material.setTextureId(textureId);
    }
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

void RenderSystem::Progress(float /*timeDelta*/)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_GREATER);
    for (auto entity : m_entities)
    {
        auto& mesh = m_ecsContainer.GetComponent<Mesh>(entity);
        auto& material = m_ecsContainer.GetComponent<Material>(entity);
        auto& transform = m_ecsContainer.GetComponent<Transform>(entity);
        auto texture = material.getTexture();
        auto shader = material.getShader();

        shader->use();
        auto textureId = material.getTextureId();
        auto vao = mesh.getVao();
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindVertexArray(vao);
        shader->setBool("drawTexture", true);
        shader->setInt("ourTexture", textureId);
        auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        auto projection = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f) * projection;
        shader->setMat4("model", getTransform(transform));
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        auto indices = mesh.getIndices();
        glDrawElements(GL_TRIANGLES, indices.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    }
}
