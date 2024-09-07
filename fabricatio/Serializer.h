#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <entt/entt.hpp>

class Serializer
{
public:
    static void serialize(const entt::registry& registry);
};

#endif // SERIALIZER_H
