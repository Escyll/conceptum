#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#pragma once

#include <set>

#include "ECSContainer.h"

class RenderSystem : public System
{
public:
    explicit RenderSystem(ECSContainer &ecsContainer, const std::set<Entity>& entities);

    void Progress(float timeDelta) override;

private:
    void loadBuffers() const;

private:
    ECSContainer& m_ecsContainer;
    std::set<Entity> m_entities;
};

#endif