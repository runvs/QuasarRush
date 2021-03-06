#include "weapon_rockets.hpp"
#include "game_properties.hpp"
#include <math_helper.hpp>

WeaponRockets::WeaponRockets()
{
    // TODO Sound
    m_shotSounds = std::make_unique<jt::SoundGroup>(
        std::vector<std::string> { "assets/sfx/rocket_fire.ogg" });
    m_shotSounds->setVolume(50);
}

void WeaponRockets::shoot(jt::Vector2 const& playerPos, jt::Vector2 const& mousePosition,
    PlayerConfig playerConfig, ShotSpawnInterface& shotSpawnInterface)
{
    if (canShoot()) {
        auto aim_direction = mousePosition - playerPos;
        jt::MathHelper::normalizeMe(aim_direction);

        float shotTimeFactor = 1.0f - (static_cast<float>(playerConfig.weaponLevel) - 1.0f) * 0.1f;
        shotTimeFactor = jt::MathHelper::clamp(shotTimeFactor, 0.2f, 1.0f);

        m_shootTimer = GP::PlayerShootTimerMissile() * shotTimeFactor;
        m_shootTimerMax = m_shootTimer;
        shotSpawnInterface.spawnShotMissile(playerPos, aim_direction, true);
        m_shotSounds->play();
    }
}

bool WeaponRockets::canShoot() { return m_shootTimer <= 0; }
void WeaponRockets::update(float elapsed) { m_shootTimer -= elapsed; }

float WeaponRockets::getReloadCompletion()
{
    if (canShoot()) {
        return 0.0f;
    }
    return m_shootTimer / m_shootTimerMax;
}
