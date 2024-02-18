#include <iostream>
#include <iterator>
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
