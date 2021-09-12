#ifndef QUASARRUSH_WEAPON_ROCKETS_HPP
#define QUASARRUSH_WEAPON_ROCKETS_HPP

#include "sound_group.hpp"
#include "weapon_interface.hpp"
#include <memory>

class WeaponRockets : public WeaponInterface {
public:
    WeaponRockets();
    void shoot(jt::Vector2 const& playerPos, jt::Vector2 const& mousePosition,
        PlayerConfig playerConfig, ShotSpawnInterface& shotSpawnInterface) override;

    void update(float elapsed) override;
    float getReloadCompletion() override;

private:
    float m_shootTimer { 0.0f };
    float m_shootTimerMax { 1.0f };
    bool canShoot();

    std::unique_ptr<jt::SoundGroup> m_shotSounds;
};

#endif // QUASARRUSH_WEAPON_ROCKETS_HPP
