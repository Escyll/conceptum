#ifndef PROGRAM_H
#define PROGRAM_H

#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class ShaderProgram
{
public:
    ShaderProgram(const std::vector<int> &shaders);

    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
    unsigned int id() const;

private:
    unsigned int m_programId;
};

#endif