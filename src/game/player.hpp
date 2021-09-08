#ifndef GAME_PLAYER_GUARD_HPP
#define GAME_PLAYER_GUARD_HPP

#include "animation.hpp"
#include "game_object.hpp"
#include "game_properties.hpp"
#include "player_config.hpp"
#include "shape.hpp"
#include "shot_spawn_interface.hpp"
#include "spawn_trail_interface.hpp"
#include "transform.hpp"
#include "weapon_interface.hpp"
#include <memory>

class Player : public jt::GameObject {
public:
    Player(ShotSpawnInterface& shotSpawnInterface, SpawnTrailInterface& spawnTrailInterface,
        PlayerConfig& pc);
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

    void shoot();

    std::shared_ptr<jt::Animation> getSprite() const;

    std::shared_ptr<Transform> getTransform();
    void setTransform(std::shared_ptr<Transform> t);

    void setProjectionPoints(std::vector<jt::Vector2>&& points);

private:
    ShotSpawnInterface& m_shotSpawnInterface;
    SpawnTrailInterface& m_spawnTrailInterface;

    std::shared_ptr<jt::Animation> m_shipSprite;
    std::shared_ptr<jt::Animation> m_flameSprite;
    std::shared_ptr<jt::Shape> m_projectionShape;
    std::shared_ptr<Transform> m_transform;
    std::vector<jt::Vector2> m_projectionPoints;
    std::shared_ptr<jt::Sprite> m_glowOverlayFlame;
    std::shared_ptr<jt::Sprite> m_glowOverlayShip;

    std::unique_ptr<WeaponInterface> m_weapon;

    // only store a reference, as we do not want to keep a copy and state_game contains player, so
    // the reference will always be valid.
    PlayerConfig& m_playerConfig;

    void updateMovement(const float elapsed);
    void updateShooting(float const elapsed);
    void updateSprite(float const elapsed);
    void updateFlame(float const elapsed);
};

#endif
