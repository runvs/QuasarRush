#ifndef QUASARRUSH_PLAYER_CONFIG_HPP
#define QUASARRUSH_PLAYER_CONFIG_HPP

#include <set>

enum eWeaponType
{
    WeaponMg,
    WeaponRockets
};

struct PlayerConfig
{
    eWeaponType weapon{WeaponMg};
    int sensorLevel{1};

    std::set<int> availableLevels {0};

};

#endif // QUASARRUSH_PLAYER_CONFIG_HPP
