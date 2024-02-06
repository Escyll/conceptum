#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#pragma once

#include <set>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "ECSContainer.h"

class RenderSystem : public System
{
public:
    explicit RenderSystem(ECSContainer &ecsContainer);

    void Progress(float timeDelta, const std::set<Entity>& entities) override;
    int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) const;
    void UseShaderProgram(int program);
    void setBool(int shaderProgram, const std::string& name, bool value) const;
    void setInt(int shaderProgram, const std::string& name, int value) const;
    void setFloat(int shaderProgram, const std::string& name, float value) const;
    void setVec3(int shaderProgram, const std::string& name, glm::vec3 value) const;
    void setMat4(int shaderProgram, const std::string& name, glm::mat4 value) const;

    void loadBuffers(const std::set<Entity>& entities) const;

private:
    ECSContainer& m_ecsContainer;
};

#endif