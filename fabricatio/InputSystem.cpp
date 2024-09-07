#include <iostream>

#include "producentis/KeyInput.h"

#include "InputSystem.h"

void InputSystem::processInput(const Input &input)
{
    m_movementInput.clear();
    m_globalInput.clear();
    for (auto keyEvent : input.keyEvents)
    {
        switch (keyEvent.key)
        {
        case PRD_KEY_ESCAPE:
            m_globalInput.push_back({
                GlobalInput::ESCAPE,
            });
            break;
        case PRD_KEY_F1:
            if (keyEvent.action == PRD_RELEASE)
            {
                m_globalInput.push_back({
                    GlobalInput::LOAD_SCENE,
                });
            }
            break;
        case PRD_KEY_F2:
            if (keyEvent.action == PRD_RELEASE)
            {
                m_globalInput.push_back({
                    GlobalInput::SAVE_SCENE,
                });
            }
            break;
        case PRD_KEY_P:
            if (keyEvent.action == PRD_RELEASE)
            {
                m_globalInput.push_back({GlobalInput::TOGGLE_MENU});
            }
            break;
        case PRD_KEY_LEFT_SHIFT:
            m_movementInput.push_back({MovementInput::SPRINT, keyEvent.action == PRD_RELEASE ? MovementInput::STOP : MovementInput::START});
            break;
        case PRD_KEY_W:
            m_movementInput.push_back({MovementInput::FORWARD, keyEvent.action == PRD_RELEASE ? MovementInput::STOP : MovementInput::START});
            break;
        case PRD_KEY_S:
            m_movementInput.push_back({MovementInput::BACKWARD, keyEvent.action == PRD_RELEASE ? MovementInput::STOP : MovementInput::START});
            break;
        case PRD_KEY_A:
            m_movementInput.push_back({MovementInput::LEFT, keyEvent.action == PRD_RELEASE ? MovementInput::STOP : MovementInput::START});
            break;
        case PRD_KEY_D:
            m_movementInput.push_back({MovementInput::RIGHT, keyEvent.action == PRD_RELEASE ? MovementInput::STOP : MovementInput::START});
            break;
        case PRD_KEY_SPACE:
            m_movementInput.push_back({MovementInput::UP, keyEvent.action == PRD_RELEASE ? MovementInput::STOP : MovementInput::START});
            break;
        case PRD_KEY_LEFT_CONTROL:
            m_movementInput.push_back({MovementInput::DOWN, keyEvent.action == PRD_RELEASE ? MovementInput::STOP : MovementInput::START});
            break;
        }
    }
    m_axisInput = {-input.deltaMouseX / 200.f, -input.deltaMouseY / 200.f};
}

std::vector<MovementInput> InputSystem::movementInput() const
{
    return m_movementInput;
}

std::vector<GlobalInput> InputSystem::globalInput() const
{
    return m_globalInput;
}

std::pair<double, double> InputSystem::axisInput() const
{
    return m_axisInput;
}
