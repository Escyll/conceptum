#include <iostream>
#include <sstream>
#include <stdexcept>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"

namespace Program
{

    template <typename Range, typename Value = typename Range::value_type>
    std::string join(Range const &elements, const char *const delimiter)
    {
        std::ostringstream os;
        auto b = begin(elements), e = end(elements);

        if (b != e)
        {
            std::copy(b, prev(e), std::ostream_iterator<Value>(os, delimiter));
            b = prev(e);
        }
        if (b != e)
        {
            os << *b;
        }

        return os.str();
    }

    std::string shadersToString(const std::vector<int> &shaders)
    {
        std::stringstream result;
        result << '[' << join(shaders, ", ") << ']';
        return result.str();
    }

    unsigned int createProgram(const std::vector<int> &shaders)
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
            throw std::runtime_error("Failed to create program with shaders " + shadersToString(shaders) + "\n" + infoLog);
        }
        std::cout << "Created program with shaders " << shadersToString(shaders) << std::endl;
        return program;
    }

}

ShaderProgram::ShaderProgram(const std::vector<int> &shaders)
{
    m_programId = Program::createProgram(shaders);
}

void ShaderProgram::use() const
{
    glUseProgram(m_programId);
}

void ShaderProgram::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_programId, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string &name, int value) const
{
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string &name, glm::vec3 value) const
{
    glUniform3f(glGetUniformLocation(m_programId, name.c_str()), value.x, value.y, value.z);
}

void ShaderProgram::setMat4(const std::string &name, glm::mat4 value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

unsigned int ShaderProgram::id() const
{
    return m_programId;
}
