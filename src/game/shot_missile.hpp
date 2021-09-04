#ifndef QUASARRUSH_MISSILE_HPP
#define QUASARRUSH_MISSILE_HPP

#include "animation.hpp"
#include "explosion_spawn_interface.hpp"
#include "game_object.hpp"
#include "shot_interface.hpp"
#include "spawn_trail_interface.hpp"
#include "sprite.hpp"
#include <memory>

class ShotMissile : public jt::GameObject, public ShotInterface {
private:
    std::shared_ptr<jt::Animation> m_animation;
    bool m_firedByPlayer;
    std::shared_ptr<Transform> m_transform;
    std::weak_ptr<Transform> m_target;
    jt::Vector2 m_targetOffset;
    SpawnTrailInterface& m_spawnTrailInterface;
    ExplosionSpawnInterface& m_explosionSpawnInterface;
    std::shared_ptr<jt::Sprite> m_glowOverlayFlame;
    float m_missileAcquisitionTime;

public:
    explicit ShotMissile(
        SpawnTrailInterface& spawnTrailInterface, ExplosionSpawnInterface& explosionSpawnInterface);
    std::shared_ptr<jt::DrawableInterface> getDrawable() override;
    float getDamageValue() override;
    std::shared_ptr<Transform> getTransform() override;
    bool getFiredByPlayer() const override;
    void hit() override;
    void setTarget(std::weak_ptr<Transform> target);

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    void setTransform(std::shared_ptr<Transform> t);
    void setFiredByPlayer(bool value);
};

#endif // QUASARRUSH_MISSILE_HPP
