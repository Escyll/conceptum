#include <string>
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
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
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

void Renderer::loadMesh(Mesh* mesh) const
{
    uint32_t VAO;
    glGenVertexArrays(1, &VAO);
    uint32_t VBO;
    glGenBuffers(1, &VBO);
    uint32_t EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferVector(GL_ARRAY_BUFFER, mesh->getVertexBuffer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferVector(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndices(), GL_STATIC_DRAW);
    // Vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mesh->setVao(VAO);
}

// TODO: view and project can be done separate?
void Renderer::drawMesh(Mesh* mesh, int shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const
{
    useShaderProgram(shader);
    glBindVertexArray(mesh->getVao());
    // auto textureId = material.getTextureId;
    // glBindTexture(GL_TEXTURE_2D, textureId);
    // setInt(shader, "ourTexture", textureId);
    setBool(shader, "drawTexture", false);
    setMat4(shader, "model", model);
    setMat4(shader, "view", view);
    setMat4(shader, "projection", projection);
    auto indices = mesh->getIndices();
    glDrawElements(GL_TRIANGLES, indices.size() * sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}

void Renderer::clearScreen() const
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_GREATER);
}

void Renderer::resizeViewport(int width, int height) const
{
    glViewport(0, 0, width, height);
}

