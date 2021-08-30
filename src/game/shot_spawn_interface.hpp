#ifndef QUASARRUSH_SHOT_SPAWN_INTERFACE_HPP
#define QUASARRUSH_SHOT_SPAWN_INTERFACE_HPP

#include "vector.hpp"

class ShotSpawnInterface
{
public:
    virtual void spawnShot(jt::Vector2 const& pos, jt::Vector2 dir, bool byPlayer) = 0;
};

#endif // QUASARRUSH_SHOT_SPAWN_INTERFACE_HPP
