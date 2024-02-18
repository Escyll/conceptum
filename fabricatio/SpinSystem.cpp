#include "SpinSystem.h"

SpinSystem::SpinSystem(ECSContainer& ecs) : m_ecs(ecs) {}

void SpinSystem::progress(float timeDelta, const std::set<Entity>& entities)
{
    for (auto entity : entities)
    {
        if (m_ecs.hasComponent<TimeSpin>(entity))
        {
            auto& timeSpin = m_ecs.getComponent<TimeSpin>(entity);
            auto& transform = m_ecs.getComponent<Transform>(entity);
            transform.rotation += timeDelta * timeSpin.rotation;
        }
    }
}
