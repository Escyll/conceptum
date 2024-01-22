#ifndef SHADERCOMPILER_H
#define SHADERCOMPILER_H

#pragma once

namespace ShaderCompiler
{

    enum class ShaderType
    {
        Vertex,
        Fragment
    };

    int compileShader(std::string path, ShaderType type);

}

#endif