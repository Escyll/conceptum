#include <numbers>

#include "PlayerControlComponent.h"
#include "Transform.h"

#include "PlayerControllerSystem.h"

PlayerControllerSystem::PlayerControllerSystem(entt::registry& registry, InputSystem& inputSystem)
    : System(registry)
    , m_inputSystem(inputSystem)
{}

void PlayerControllerSystem::progress(float timeDelta)
{
    auto view = m_registry.view<PlayerControlComponent, Transform>();
    auto movementInput = m_inputSystem.movementInput();
    constexpr float cameraBaseSpeed = 6;
    float speedModifier = 1.0;
    for (auto& input : movementInput)
    {
        switch (input.movement)
        {
        case MovementInput::FORWARD:
            if (input.action == MovementInput::START)
            {
                m_motion.y = 1;
            }
            if (input.action == MovementInput::STOP)
            {
                m_motion.y = 0;
            }
            break;
        case MovementInput::BACKWARD:
            m_motion.y = input.action == MovementInput::START ? -1 : 0;
            break;
        case MovementInput::LEFT:
            m_motion.x = input.action == MovementInput::START ? -1 : 0;
            break;
        case MovementInput::RIGHT:
            m_motion.x = input.action == MovementInput::START ? 1 : 0;
            break;
        case MovementInput::UP:
            m_motion.z = input.action == MovementInput::START ? 1 : 0;
            break;
        case MovementInput::DOWN:
            m_motion.z = input.action == MovementInput::START ? -1 : 0;
            break;
        case MovementInput::SPRINT:
            speedModifier = input.action == MovementInput::START ? 2.0 : 1.0;
            break;
        }
    }
    if (glm::length(m_motion) > 0.1)
    {
        auto motion = glm::normalize(m_motion) * cameraBaseSpeed * speedModifier * timeDelta;
        for (const auto& [pcc, t] : view.each())
        {
            t.location += motion;
        };
    }
    auto axisInput = m_inputSystem.axisInput();
    auto limitPitch = [](double value) { return std::max(-std::numbers::pi * 0.5, std::min(value, std::numbers::pi * 0.5)); };
    for (const auto& [pcc, t] : view.each())
    {
        t.rotation.x = limitPitch(t.rotation.x + axisInput.second);
        t.rotation.z += axisInput.first;
    };
}
