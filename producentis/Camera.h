#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "producentis/producentis.h"

class producentis_API Camera
{
public:
    glm::mat4 rotation() const;
    glm::mat4 translation() const;
    glm::mat4 view() const;
    glm::mat4 projection() const;

    glm::vec3 right() const;
    glm::vec3 up() const;
    glm::vec3 forward() const;

    void moveRight(float delta);
    void moveForward(float delta);
    void moveUp(float delta);

    void pitch(float angle);
    void yaw(float angle);
    void roll(float angle);

    void setPosition(const glm::vec3 &position);
    void setFov(float fov);
    void setAspect(float aspect);
    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);

private:
    glm::vec3 m_position{0, 0, 0};
    float m_fov = 70.0f;
    float m_aspect = 1920.f / 1080.f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 2000.f;
    float m_pitch = 0.f;
    float m_yaw = 0.f;
    float m_roll = 0.f;
};

#endif