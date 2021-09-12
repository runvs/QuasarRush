#ifndef QUASARRUSH_WEAPON_MG_HPP
#define QUASARRUSH_WEAPON_MG_HPP

#include "sound.hpp"
#include "sound_group.hpp"
#include "weapon_interface.hpp"
#include <memory>

class WeaponMg : public WeaponInterface {
public:
    WeaponMg();
    void shoot(jt::Vector2 const& shooterPos, jt::Vector2 const& mousePosition,
        PlayerConfig playerConfig, ShotSpawnInterface& shotSpawnInterface) override;

    void update(float elapsed) override;
    float getReloadCompletion() override;

private:
    int m_shotCounter { 0 };
    float m_reloadTimer { 0.0f };
    float m_shootTimer { 0.0f };
    float m_reloadTimerMax { 1.0f };

    bool canShoot();

    std::unique_ptr<jt::SoundGroup> m_shotSounds;
};

#endif // QUASARRUSH_WEAPON_MG_HPP
