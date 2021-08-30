#ifndef GAME_PLAYER_GUARD_HPP
#define GAME_PLAYER_GUARD_HPP

#include "animation.hpp"
#include "game_object.hpp"
#include "game_properties.hpp"
#include "shape.hpp"
#include "shot_spawn_interface.hpp"
#include "transform.hpp"
#include <memory>

class Player : public jt::GameObject {
public:
    Player(ShotSpawnInterface& shotSpawnInterface);
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

    bool canShoot();
    void shoot();

    std::shared_ptr<jt::Animation> getSprite() const;

    std::shared_ptr<Transform> getTransform();
    void setTransform(std::shared_ptr<Transform> t);

    void setProjectionPoints(std::vector<jt::Vector2>&& points);

private:
    ShotSpawnInterface& m_shotSpawnInterface;
    std::shared_ptr<jt::Animation> m_shipSprite;
    std::shared_ptr<jt::Animation> m_flameSprite;
    std::shared_ptr<jt::Shape> m_projectionShape;
    std::shared_ptr<Transform> m_transform;
    std::vector<jt::Vector2> m_projectionPoints;

    float m_shootTimer = 0.0f;

    void updateMovement(const float elapsed);
};

#endif
