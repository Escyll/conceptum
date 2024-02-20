#ifndef SPIN_SYSTEM_H
#define SPIN_SYSTEM_H

#include <set>
#include <glm/common.hpp>
#include <glm/ext/vector_float3.hpp>

#include "ECSContainer.h"
#include "Transform.h"

struct TimeSpin
{
    glm::vec3 rotation{ 0.0f, 0.0f, 1.0f };
};

class SpinSystem : public System
{
public:
    SpinSystem(ECSContainer& ecs);
    void progress(float timeDelta, const std::set<Entity>& entities) override;
    
private:
    ECSContainer& m_ecs;
};

#endif // !SPIN_SYSTEM_H
