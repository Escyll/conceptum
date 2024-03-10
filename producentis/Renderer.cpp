#include <string>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderCompiler.h"
#include "ShaderProgram.h"

#include "Renderer.h"

extern "C" Renderer* CreateRenderer()
{
    return new Renderer;
}

extern "C" void DestroyRenderer(Renderer* renderer)
{
    delete renderer;
}

void Renderer::initialize() const
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
       throw std::runtime_error("Failed to initialize GLAD");
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
}

int Renderer::createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) const
{
    int vertexShader = ShaderCompiler::compileShader(vertexShaderSource, ShaderCompiler::ShaderType::Vertex);
    int fragmentShader = ShaderCompiler::compileShader(fragmentShaderSource, ShaderCompiler::ShaderType::Fragment);
    
    auto program = Program::createProgram({vertexShader, fragmentShader});

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void Renderer::useShaderProgram(int program) const
{
    glUseProgram(program);
}

void Renderer::setBool(int shaderProgram, const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void Renderer::setInt(int shaderProgram, const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Renderer::setFloat(int shaderProgram, const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Renderer::setVec3(int shaderProgram, const std::string& name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), value.x, value.y, value.z);
}

void Renderer::setMat4(int shaderProgram, const std::string& name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

template<typename T>
void glBufferVector(GLenum target, const std::vector<T>& data, GLenum usage)
{
    glBufferData(target, data.size() * sizeof(T), &data[0], usage);
}

void loadTexture(Texture* texture)
{
    uint32_t textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto width = texture->getWidth();
    auto height = texture->getHeight();
    auto channels = texture->getChannels();
    auto data = texture->getData();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    auto colorSpace = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, colorSpace, texture->getWidth(), texture->getHeight(), 0, colorSpace, GL_UNSIGNED_BYTE, texture->getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    texture->setId(textureId);
}

void Renderer::loadMesh(Mesh* mesh) const
{       
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferVector(GL_ARRAY_BUFFER, mesh->getVertexBuffer(), GL_STATIC_DRAW);
    auto& materialsMap = mesh->getMaterials();
    for (auto& subMesh : mesh->getSubMeshes())
    {
        uint32_t VAO;
        glGenVertexArrays(1, &VAO);
        subMesh.vao = VAO;
        uint32_t EBO;
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, subMesh.indexCount * sizeof(uint32_t), &(mesh->getIndices()[subMesh.startIndex]), GL_STATIC_DRAW);

        // Vertex
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        // UV
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Normal
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // Tangents
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
        // Bitangents
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void *)(11 * sizeof(float)));
        glEnableVertexAttribArray(4);
    
        auto material = materialsMap[subMesh.materialName].get();
        if (material != nullptr)
        {
            loadTexture(materialsMap[subMesh.materialName]->diffuseTexture.get());
            if (materialsMap[subMesh.materialName]->normalMap)
            {
                loadTexture(materialsMap[subMesh.materialName]->normalMap.get());
            }
        }
    }
}

// TODO: view and project can be done separate?
void Renderer::drawMesh(Mesh* mesh, int shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
    useShaderProgram(shader);
    // TODO: Figure out what useShaderProgram should do
    auto indices = mesh->getIndices();
    auto& materialsMap = mesh->getMaterials();
    for (auto& subMesh : mesh->getSubMeshes())
    {
        auto* material = materialsMap[subMesh.materialName].get();
        setInt(shader, "diffuseTexture", 0);
        setVec3(shader, "diffuseScale", material->diffuseScale);
        if (material && material->diffuseTexture)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, material->diffuseTexture->getId());
        }
        setInt(shader, "normalMap", 1);
        setBool(shader, "useNormalMap", material->normalMap != nullptr);
        setFloat(shader, "normalStrength", material->normalStrength);
        setVec3(shader, "normalScale", material->normalScale);
        if (material && material->normalMap)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, material->normalMap->getId());
        }
        setVec3(shader, "lightPos", glm::vec3(0, 0, 3));
        // setVec3(shader, "lightColor", glm::vec3(1, 0.799, 0.825));
        setVec3(shader, "lightColor", glm::vec3(1, 1, 1));
        setFloat(shader, "specularExponent", material->specularExponent);
        setVec3(shader, "ambientColor", material->ambientColor);
        setVec3(shader, "diffuseColor", material->ambientColor);
        setVec3(shader, "specularColor", material->ambientColor);
        setVec3(shader, "viewPos", glm::vec3(0, 0, 0));
        setMat4(shader, "model", model);
        setMat4(shader, "view", view);
        setMat4(shader, "projection", projection);
        glBindVertexArray(subMesh.vao);
        glDrawElements(GL_TRIANGLES, subMesh.indexCount * sizeof(uint32_t), GL_UNSIGNED_INT, 0);
    }
}

void Renderer::clearScreen() const
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::resizeViewport(int width, int height) const
{
    glViewport(0, 0, width, height);
}

