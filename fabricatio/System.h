#ifndef SYSTEM_H
#define SYSTEM_H

#include <entt/entt.hpp>

class System {
public:
    System(entt::registry& registry)
        : m_registry(registry)
    {}
    ~System() = default;
    virtual void progress(float timeDelta) = 0;
protected:
    entt::registry& m_registry;
};

#endif
