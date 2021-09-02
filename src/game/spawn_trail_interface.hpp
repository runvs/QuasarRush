#ifndef QUASARRUSH_SPAWN_TRAIL_INTERFACE_HPP
#define QUASARRUSH_SPAWN_TRAIL_INTERFACE_HPP

#include "vector.hpp"

class SpawnTrailInterface
{
public:
    virtual void spawnTrail(jt::Vector2 pos) = 0;
};

#endif // QUASARRUSH_SPAWN_TRAIL_INTERFACE_HPP
