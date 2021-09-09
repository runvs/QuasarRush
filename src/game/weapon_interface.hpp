#ifndef QUASARRUSH_WEAPON_INTERFACE_HPP
#define QUASARRUSH_WEAPON_INTERFACE_HPP

#include "player_config.hpp"
#include "shot_spawn_interface.hpp"
#include "vector.hpp"

class WeaponInterface
{
public:
    virtual ~WeaponInterface() = default;
    virtual void shoot(jt::Vector2 const& playerPos, jt::Vector2 const& mousePosition,
        PlayerConfig playerConfig, ShotSpawnInterface& shotSpawnInterface)
        = 0;
    virtual void update(float elapsed) = 0;
    virtual float getReloadCompletion() = 0;
};

#endif // QUASARRUSH_WEAPON_INTERFACE_HPP
