#ifndef SHOT_GUARD_HPP
#define SHOT_GUARD_HPP

#include "explosion_spawn_interface.hpp"
#include "game_object.hpp"
#include "shot_interface.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>

class ShotMg : public jt::GameObject, public ShotInterface {
private:
    std::shared_ptr<jt::Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;
    ExplosionSpawnInterface& m_explosionSpawnInterface;

    bool m_fired_by_player { true };

public:
    explicit ShotMg::ShotMg(ExplosionSpawnInterface& explosionSpawnInterface);

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    std::shared_ptr<Transform> getTransform() override;
    void setTransform(std::shared_ptr<Transform> t);
    void setFiredByPlayer(bool value);
    bool getFiredByPlayer() const override;

private:
    void hit() override;
};

#endif
