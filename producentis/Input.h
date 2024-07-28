#ifndef INPUT_H
#define INPUT_H

#include <memory>

#include "KeyEvent.h"

struct Input
{
    std::vector<KeyEvent> keyEvents;
    float deltaMouseX, deltaMouseY;
};

#endif