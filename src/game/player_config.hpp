#ifndef QUASARRUSH_PLAYER_CONFIG_HPP
#define QUASARRUSH_PLAYER_CONFIG_HPP

#include <set>

enum eWeaponType { WeaponMg, WeaponRockets };

struct PlayerConfig {
    eWeaponType weapon { WeaponMg };
    int sensorLevel { 1 };

    std::set<int> availableLevels { 0, 1, 2, 3, 4 };
};

#endif // QUASARRUSH_PLAYER_CONFIG_HPP
