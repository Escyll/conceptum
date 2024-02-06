#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderCompiler.h"
#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>

RenderSystem::RenderSystem(ECSContainer &ecsContainer)
    : m_ecsContainer(ecsContainer)
{
}

void RenderSystem::loadBuffers(const std::set<Entity>& entities) const
{
    for (auto entity : entities)
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

void RenderSystem::Progress(float /*timeDelta*/, const std::set<Entity>& entities)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_GREATER);
    for (auto entity : entities)
    {
        auto& mesh = m_ecsContainer.GetComponent<Mesh>(entity);
        auto& material = m_ecsContainer.GetComponent<Material>(entity);
        auto& transform = m_ecsContainer.GetComponent<Transform>(entity);
        auto shader = material.getShader();

        UseShaderProgram(shader);

        auto textureId = material.getTextureId();
        auto vao = mesh.getVao();
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindVertexArray(vao);
        setBool(shader, "drawTexture", true);
        setInt(shader, "ourTexture", textureId);
        auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        auto projection = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f));
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f) * projection;
        setMat4(shader, "model", getTransform(transform));
        setMat4(shader, "view", view);
        setMat4(shader, "projection", projection);
        auto indices = mesh.getIndices();
        glDrawElements(GL_TRIANGLES, indices.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    }
}

int RenderSystem::CreateShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) const
{
    int vertexShader = ShaderCompiler::compileShader(vertexShaderSource, ShaderCompiler::ShaderType::Vertex);
    int fragmentShader = ShaderCompiler::compileShader(fragmentShaderSource, ShaderCompiler::ShaderType::Fragment);
    
    auto program = Program::createProgram({vertexShader, fragmentShader});

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void RenderSystem::UseShaderProgram(int program)
{
    glUseProgram(program);
}

void RenderSystem::setBool(int shaderProgram, const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void RenderSystem::setInt(int shaderProgram, const std::string& name, int value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void RenderSystem::setFloat(int shaderProgram, const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void RenderSystem::setVec3(int shaderProgram, const std::string& name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y, value.z);
}

void RenderSystem::setMat4(int shaderProgram, const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
