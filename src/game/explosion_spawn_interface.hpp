#ifndef EXPLOSION_SPAWN_INTERFACE_GUARD_HPP
#define EXPLOSION_SPAWN_INTERFACE_GUARD_HPP

#include "vector.hpp"

class ExplosionSpawnInterface {
public:
    virtual void spawnExplosion(jt::Vector2 const& position) = 0;
};

#endif // EXPLOSION_SPAWN_INTERFACE_GUARD_HPP
