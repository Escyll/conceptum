#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <vector>

#include "producentis/Input.h"

struct GlobalInput
{
    enum Action
    {
        ESCAPE,
        TOGGLE_MENU
    };
    Action action;
};

struct MovementInput
{
    enum Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        SPRINT
    };
    enum Action
    {
        START,
        STOP
    };
    Movement movement;
    Action action;
};

class InputSystem
{
public:
    void processInput(const Input &input);
    std::vector<MovementInput> movementInput() const;
    std::vector<GlobalInput> globalInput() const;
    std::pair<double, double> axisInput() const;

private:
    std::vector<MovementInput> m_movementInput{};
    std::vector<GlobalInput> m_globalInput{};
    std::pair<double, double> m_axisInput{0.0, 0.0};
};

#endif
