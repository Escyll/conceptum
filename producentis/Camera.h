#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "producentis/producentis.h"

struct producentis_API Camera
{
    glm::mat4 projection() const;

    void setFov(float fov);
    void setAspect(float aspect);
    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);

    float fov = 70.0f;
    // float aspect = 1920.f / 1080.f;
    float aspect = 1080.f / 1080.f;
    float nearPlane = 0.1f;
    float farPlane = 2000.f;
};

#endif
