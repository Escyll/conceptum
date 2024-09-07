#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>

#include "producentis.h"

struct Light
{
    float intensity;
    glm::vec3 color;
};

struct producentis_API PointLight : public Light
{};

struct producentis_API PositionedPointLight
{
    PointLight pointLight;
    glm::mat4 transform;
};

struct producentis_API Lights
{
    std::vector<PositionedPointLight> pointLights;
};

#endif // LIGHTS_H
