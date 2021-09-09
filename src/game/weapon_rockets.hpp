#ifndef QUASARRUSH_WEAPON_ROCKETS_HPP
#define QUASARRUSH_WEAPON_ROCKETS_HPP

#include "weapon_interface.hpp"

class WeaponRockets : public WeaponInterface {
public:
    void shoot(jt::Vector2 const& playerPos, jt::Vector2 const& mousePosition,
        PlayerConfig playerConfig, ShotSpawnInterface& shotSpawnInterface) override;

    void update(float elapsed) override;
private:
    float m_shootTimer{0.0f};
    float m_shootTimerMax{1.0f};
    bool canShoot();

public:
    float getReloadCompletion() override;
};

#endif // QUASARRUSH_WEAPON_ROCKETS_HPP
