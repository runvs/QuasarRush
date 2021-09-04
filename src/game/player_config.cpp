#include "player_config.hpp"

bool playerConfigHasPointsToSpend(PlayerConfig const& pc) { return pc.pointsToSpend > 0; }
void playerConfigIncreaseSensors(PlayerConfig& pc)
{
    if (!playerConfigHasPointsToSpend(pc)) {
        return;
    }
    pc.pointsToSpend--;
    pc.sensorLevel++;
}

void playerConfigIncreaseEngine(PlayerConfig& pc)
{
    if (!playerConfigHasPointsToSpend(pc)) {
        return;
    }
    pc.pointsToSpend--;
    pc.engineLevel++;
}
void playerConfigSwitchToMg(PlayerConfig& pc) { pc.weapon = WeaponMg; }
void playerConfigSwitchToMissile(PlayerConfig& pc) { pc.weapon = WeaponRockets; }

bool playerConfigCanSwitchToMg(PlayerConfig const& pc)
{
    if (pc.weapon == WeaponMg) {
        return false;
    }
    if (pc.availableWeapons.find(WeaponMg) == pc.availableWeapons.end()) {
        return false;
    }
    return true;
}
bool playerConfigCanSwitchToMissile(PlayerConfig const& pc)
{
    if (pc.weapon == WeaponRockets) {
        return false;
    }
    if (pc.availableWeapons.find(WeaponRockets) == pc.availableWeapons.end()) {
        return false;
    }
    return true;
}
