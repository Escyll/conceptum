#include "Object.h"

Object::Object(ObjectRenderer *objectRenderer, const Mesh &mesh)
    : m_objectRenderer(objectRenderer), m_mesh(mesh)
{
}

Object::~Object()
{
}

ObjectRenderer *Object::getObjectRenderer() const
{
    return m_objectRenderer;
}

glm::mat4 Object::getTransform() const
{
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, m_location);
    transform = glm::rotate(transform, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::rotate(transform, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::scale(transform, m_scale);
    return transform;
}

void Object::setLocation(const glm::vec3 &location)
{
    m_location = location;
}

void Object::setScale(const glm::vec3 &scale)
{
    m_scale = scale;
}

void Object::setRotation(const glm::vec3 &rotation)
{
    m_rotation = rotation;
}

Mesh *Object::getMesh()
{
    return &m_mesh;
}
