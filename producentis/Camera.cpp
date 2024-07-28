#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Camera.h"

glm::mat4 Camera::rotation() const
{
    glm::quat qPitch = glm::angleAxis(-m_pitch, glm::vec3(1, 0, 0));
    glm::quat qYaw = glm::angleAxis(-m_yaw, glm::vec3(0, 0, 1));
    glm::quat qRoll = glm::angleAxis(-m_roll, glm::vec3(0, 1, 0));

    glm::quat orientation = qPitch * qYaw;
    return glm::toMat4(glm::normalize(orientation));
}

glm::mat4 Camera::translation() const
{
    return glm::translate(glm::mat4(1), -m_position);
}

glm::mat4 Camera::view() const
{
    auto rotation = this->rotation();
    auto translation = this->translation();
    return rotation * translation;
}

glm::mat4 Camera::projection() const
{
    auto projection = glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3{1.f, 0.f, 0.f});
    return glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane) * projection;
}

glm::vec3 Camera::right() const
{
    return glm::vec3(glm::row(rotation(), 0));
}

glm::vec3 Camera::up() const
{
    return glm::vec3(glm::row(rotation(), 2));
}

glm::vec3 Camera::forward() const
{
    return glm::vec3(glm::row(rotation(), 1));
}

void Camera::moveRight(float delta)
{
    m_position += delta * right();
}

void Camera::moveForward(float delta)
{
    m_position += delta * forward();
}

void Camera::moveUp(float delta)
{
    m_position += delta * up();
}

void Camera::pitch(float angle)
{
    m_pitch += angle;
    if (glm::degrees(m_pitch) < -90.f)
        m_pitch = glm::radians(-90.f);
    if (glm::degrees(m_pitch) > 90.f)
        m_pitch = glm::radians(90.f);
}

void Camera::yaw(float angle)
{
    m_yaw += angle;
}

void Camera::roll(float angle)
{
    m_roll += angle;
}

void Camera::setPosition(const glm::vec3 &position)
{
    m_position = position;
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
