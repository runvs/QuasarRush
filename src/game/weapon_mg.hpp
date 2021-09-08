#ifndef QUASARRUSH_WEAPON_MG_HPP
#define QUASARRUSH_WEAPON_MG_HPP

#include "weapon_interface.hpp"

class WeaponMg : public WeaponInterface{
public:
    void shoot(jt::Vector2 const& shooterPos, jt::Vector2 const& mousePosition,
        PlayerConfig playerConfig, ShotSpawnInterface& shotSpawnInterface) override;
    bool canShoot() override;
    void update(float elapsed) override;
private:
    int m_shotCounter{0};
    float m_shootTimer{0.0f};
};

#endif // QUASARRUSH_WEAPON_MG_HPP
