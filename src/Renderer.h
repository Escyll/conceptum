#ifndef RENDERER_H
#define RENDERER_H

#pragma once

#include <vector>

#include "Object.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void loadBuffers(const std::vector<Object *> objects) const;

    void render(const std::vector<Object *> objects);
    void setTime(float time);

private:
    float m_time;
};

#endif