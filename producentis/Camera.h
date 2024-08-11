#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "producentis/producentis.h"

class producentis_API Camera
{
public:
    glm::mat4 projection() const;

    void setFov(float fov);
    void setAspect(float aspect);
    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);

private:
    float m_fov = 70.0f;
    float m_aspect = 1920.f / 1080.f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 2000.f;
};

#endif
