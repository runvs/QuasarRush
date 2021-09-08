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

void playerConfigIncreaseWeapon(PlayerConfig& pc) {
    if (!playerConfigHasPointsToSpend(pc)) {
        return;
    }
    pc.pointsToSpend--;
    pc.weaponLevel++;
}


void playerConfigSwitchToMg(PlayerConfig& pc) { pc.weapon = WeaponTypeMg; }
void playerConfigSwitchToMissile(PlayerConfig& pc) { pc.weapon = WeaponTypeRockets; }

bool playerConfigCanSwitchToMg(PlayerConfig const& pc)
{
    if (pc.weapon == WeaponTypeMg) {
        return false;
    }
    if (pc.availableWeapons.find(WeaponTypeMg) == pc.availableWeapons.end()) {
        return false;
    }
    return true;
}
bool playerConfigCanSwitchToMissile(PlayerConfig const& pc)
{
    if (pc.weapon == WeaponTypeRockets) {
        return false;
    }
    if (pc.availableWeapons.find(WeaponTypeRockets) == pc.availableWeapons.end()) {
        return false;
    }
    return true;
}

