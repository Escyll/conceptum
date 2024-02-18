#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#pragma once

#include <set>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "ECSContainer.h"
#include "producentis/Renderer.h"

class RenderSystem : public System
{
public:
    RenderSystem(ECSContainer &ecsContainer, Renderer* renderer);

    void progress(float timeDelta, const std::set<Entity>& entities) override;

private:
    ECSContainer& m_ecsContainer;
    Renderer* m_renderer;
};

#endif
