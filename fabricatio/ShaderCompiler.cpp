#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/glad.h>

#include "ShaderCompiler.h"

namespace ShaderCompiler
{

    std::string readShader(std::string path)
    {
        using namespace std::string_literals;
        std::ifstream t(path);
        if (t.fail())
        {
            std::cout << "Could not open "s + path << std::endl;
        }
        std::stringstream buffer;
        buffer << t.rdbuf();
        return buffer.str();
    }

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

    int compileShader(std::string path, ShaderType type)
    {
        auto shaderSource = readShader(path);

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
            throw std::runtime_error("Shader compilation failed for " + path + "\n" + infoLog);
        }
        std::cout << "Compiled shader " << shader << " from source " << path << std::endl;
        return shader;
    }

}