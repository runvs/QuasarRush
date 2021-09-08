#include "weapon_mg.hpp"
#include "game_properties.hpp"
#include <math_helper.hpp>

void WeaponMg::shoot(jt::Vector2 const& shooterPos, jt::Vector2 const& mousePosition,
    PlayerConfig playerConfig, ShotSpawnInterface& shotSpawnInterface)
{
    auto aimDirection = mousePosition - shooterPos;
    jt::MathHelper::normalizeMe(aimDirection);
    m_shotCounter++;

    float shotTimeFactor = 1.0f - (static_cast<float>(playerConfig.weaponLevel) - 1.0f) * 0.1f;
    shotTimeFactor = jt::MathHelper::clamp(shotTimeFactor, 0.2f, 1.0f);

    // TODO Refactor to avoid ugly if statement. Use proper OOP
    if (playerConfig.weapon == WeaponTypeMg) {
        m_shootTimer = GP::PlayerShootTimerMg() * shotTimeFactor;
        jt::Vector2 const orthogonal_aim_direction { aimDirection.y(), -aimDirection.x() };
        auto startPos = shooterPos
            + 6.0f * orthogonal_aim_direction * ((m_shotCounter % 2 == 0) ? -1.0f : 1.0f);
        shotSpawnInterface.spawnShotMg(startPos, aimDirection, true);
    }
}

bool WeaponMg::canShoot() { return m_shootTimer <= 0; }
void WeaponMg::update(float elapsed) { m_shootTimer -= elapsed; }
