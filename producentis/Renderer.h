#ifndef PRODUCENTIS_RENDERER_H
#define PRODUCENTIS_RENDERER_H

#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

class Renderer
{
public:
    void initialize() const;
    int createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) const;
    void useShaderProgram(int program) const;
    void setBool(int shaderProgram, const std::string& name, bool value) const;
    void setInt(int shaderProgram, const std::string& name, int value) const;
    void setFloat(int shaderProgram, const std::string& name, float value) const;
    void setVec3(int shaderProgram, const std::string& name, glm::vec3 value) const;
    void setMat4(int shaderProgram, const std::string& name, glm::mat4 value) const;
    void loadMesh(Mesh* mesh) const;
    void drawMesh(Mesh* mesh, int shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;
    void clearScreen() const;
    void resizeViewport(int width, int height) const;
};

#endif
