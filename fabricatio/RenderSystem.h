#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <entt/entt.hpp>

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>


#include "System.h"

class RenderSystem : public System
{
public:
    RenderSystem(entt::registry &registry);

    void progress(float timeDelta) override;
    glm::vec4 clearColor{176.f / 255.f, 229.f / 255.f, 255.f / 255.f, 1.f};
};

#endif
