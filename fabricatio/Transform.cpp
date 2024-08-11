#include <glm/gtc/quaternion.hpp>

#include "Transform.h"

glm::mat4 Transform::transformation() const
{
    glm::quat pitch = glm::angleAxis(rotation.x, glm::vec3(1, 0, 0));
    glm::quat yaw = glm::angleAxis(rotation.z, glm::vec3(0, 0, 1));
    glm::quat roll = glm::angleAxis(rotation.y, glm::vec3(0, 1, 0));
    auto rotation = glm::mat4_cast(glm::normalize(roll * pitch * yaw));
    auto translation = glm::translate(glm::mat4(1), location);
    return rotation * translation;
}
