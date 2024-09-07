#include <stdexcept>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"

namespace Program {
unsigned int createProgram(const std::vector<int>& shaders)
{
    unsigned int program = glCreateProgram();
    for (auto shader : shaders)
    {
        glAttachShader(program, shader);
    }
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        throw std::runtime_error(std::string("Failed to create program with shaders \n") + infoLog);
    }
    return program;
    }

    } // namespace Program
