#include "SpinSystem.h"

SpinSystem::SpinSystem(ECSContainer& ecs) : m_ecs(ecs) {}

void SpinSystem::Progress(float timeDelta, const std::set<Entity>& entities)
{
    for (auto entity : entities)
    {
        auto& timeSpin = m_ecs.GetComponent<TimeSpin>(entity);
        auto& transform = m_ecs.GetComponent<Transform>(entity);
        transform.rotation += timeDelta * timeSpin.rotation;
    }
}