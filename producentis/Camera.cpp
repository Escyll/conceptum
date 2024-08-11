#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

glm::mat4 Camera::projection() const
{
    auto projection = glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3{1.f, 0.f, 0.f});
    return glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane) * projection;
}

void Camera::setFov(float fov)
{
    m_fov = fov;
}

void Camera::setAspect(float aspect)
{
    m_aspect = aspect;
}

void Camera::setNearPlane(float nearPlane)
{
    m_nearPlane = nearPlane;
}

void Camera::setFarPlane(float farPlane)
{
    m_farPlane = farPlane;
}
