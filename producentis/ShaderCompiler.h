#ifndef SHADERCOMPILER_H
#define SHADERCOMPILER_H

#pragma once

#include <string>

namespace ShaderCompiler
{

    enum class ShaderType
    {
        Vertex,
        Fragment
    };

    int compileShader(const std::string& path, ShaderType type);

}

#endif