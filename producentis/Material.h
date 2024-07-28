#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include <string>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "producentis.h"

struct producentis_API Material
{
    Material() = default;
    Material(Material &&) = default;
    std::string name;
    float specularExponent = 32;
    glm::vec3 ambientColor{1.0, 1.0, 1.0};
    glm::vec3 diffuseColor{1.0, 1.0, 1.0};
    glm::vec3 specularColor{1.0, 1.0, 1.0};
    glm::vec3 diffuseScale{1.0, 1.0, 1.0};
    float normalStrength = 1.0;
    glm::vec3 normalScale{1.0, 1.0, 1.0};

    std::unique_ptr<Texture> diffuseTexture;
    std::unique_ptr<Texture> normalMap;
    int shader;
};

#endif
