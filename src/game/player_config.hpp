#ifndef QUASARRUSH_PLAYER_CONFIG_HPP
#define QUASARRUSH_PLAYER_CONFIG_HPP

#include <set>
#include <string>

enum eWeaponType { WeaponMg, WeaponRockets };

struct PlayerConfig {
    eWeaponType weapon { WeaponMg };
    int sensorLevel { 1 };
    int engineLevel { 1 };
    int weaponLevel { 1 };

    int pointsToSpend { 0 };

    std::set<int> availableLevels { 0 };
    std::set<eWeaponType> availableWeapons { WeaponMg, WeaponRockets };
    std::string selectedLevelFilename { "" };
};

bool playerConfigHasPointsToSpend(PlayerConfig const& pc);

void playerConfigIncreaseSensors(PlayerConfig& pc);
void playerConfigIncreaseEngine(PlayerConfig& pc);
void playerConfigIncreaseWeapon(PlayerConfig& pc);

bool playerConfigCanSwitchToMg(PlayerConfig const& pc);
bool playerConfigCanSwitchToMissile(PlayerConfig const& pc);

void playerConfigSwitchToMg(PlayerConfig& pc);
void playerConfigSwitchToMissile(PlayerConfig& pc);

#endif // QUASARRUSH_PLAYER_CONFIG_HPP
