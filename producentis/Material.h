#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include <string>
#include <memory>

#include "Texture.h"

class Material
{
public:
    Material() = default;
    Material(Material&) = delete;
    Material(Material&&);
    Material(const std::string& name, std::unique_ptr<Texture>&& diffuseTexture);
    Material& operator=(Material&& material);

    std::string getName() const;
    int getShader() const;
    void setShader(int shader);
    Texture* getDiffuseTexture() const;

private:
    int m_shader;
    std::string m_name;
    std::unique_ptr<Texture> m_diffuseTexture;
};

#endif
