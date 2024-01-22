#ifndef OBJECT_H
#define OBJECT_H

#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "ObjectRenderer.h"
#include "Mesh.h"

class Object
{
public:
    Object(ObjectRenderer *objectRenderer, const Mesh &mesh);
    ~Object();

    ObjectRenderer *getObjectRenderer() const;
    glm::mat4 getTransform() const;

    void setLocation(const glm::vec3 &location);
    void setScale(const glm::vec3 &scale);
    void setRotation(const glm::vec3 &rotation);
    Mesh *getMesh();

private:
    ObjectRenderer *m_objectRenderer;
    Mesh m_mesh;
    glm::vec3 m_location = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};
    glm::vec3 m_rotation = {0.0f, 0.0f, 0.0f};
};

#endif