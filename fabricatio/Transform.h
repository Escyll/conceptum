#ifndef TRANSFORM_H
#define TRANSFORM_H

#pragma once

#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
    glm::vec3 location = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};

    glm::mat4 transformation() const;
};

#endif