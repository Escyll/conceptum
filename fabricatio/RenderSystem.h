#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#pragma once

#include <set>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "producentis/Camera.h"

#include "ECSContainer.h"

class RenderSystem : public System
{
public:
    RenderSystem(ECSContainer &ecsContainer, Camera &camera);

    void progress(float timeDelta, const std::set<Entity> &entities) override;

private:
    ECSContainer &m_ecsContainer;
    Camera &m_camera;
};

#endif
