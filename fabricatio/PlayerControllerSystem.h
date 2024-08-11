#ifndef PLAYER_CONTROLLER_SYSTEM_H
#define PLAYER_CONTROLLER_SYSTEM_H

#include <glm/glm.hpp>

#include "System.h"
#include "InputSystem.h"

class PlayerControllerSystem : public System
{
public:
    PlayerControllerSystem(entt::registry& registry, InputSystem& inputSystem);
    void progress(float timeDelta) override;

private:
    InputSystem& m_inputSystem;
    glm::vec3 m_motion{0., 0., 0.};
};

#endif
