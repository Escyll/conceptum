#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/glad.h>

#include "ShaderCompiler.h"

namespace ShaderCompiler
{       
    GLuint ShaderTypeToGLuint(ShaderType type)
    {
        switch (type)
        {
        case ShaderType::Vertex:
            return GL_VERTEX_SHADER;
            break;
        case ShaderType::Fragment:
            return GL_FRAGMENT_SHADER;
            break;
        default:
            return -1;
            break;
        }
    }

    int compileShader(const std::string& shaderSource, ShaderType type)
    {
        unsigned int shader = glCreateShader(ShaderTypeToGLuint(type));
        const char *c_str;
        glShaderSource(shader, 1, &(c_str = shaderSource.c_str()), NULL);
        glCompileShader(shader);

        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            throw std::runtime_error(std::string("Shader compilation failed \n") + infoLog);
        }
        return shader;
    }
}
