#ifndef QUASARRUSH_SHOT_SPAWN_INTERFACE_HPP
#define QUASARRUSH_SHOT_SPAWN_INTERFACE_HPP

#include "vector.hpp"

class ShotSpawnInterface
{
public:
    virtual void spawnShotMg(jt::Vector2 const& pos, jt::Vector2 const& dir, bool byPlayer) = 0;
    virtual void spawnShotMissile(jt::Vector2 const& pos, jt::Vector2 const& dir, bool byPlayer) = 0;
};

#endif // QUASARRUSH_SHOT_SPAWN_INTERFACE_HPP
