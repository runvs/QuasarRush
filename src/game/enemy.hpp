#ifndef QUASARRUSH_ENEMY_HPP
#define QUASARRUSH_ENEMY_HPP

#include "animation.hpp"
#include "game_object.hpp"
#include "shape.hpp"
#include "transform.hpp"

class EnemyAI;

class Enemy : public jt::GameObject {
    std::shared_ptr<jt::Animation> m_shipSprite;
    std::shared_ptr<jt::Animation> m_flameSprite;
    std::shared_ptr<Transform> m_transform;

    std::shared_ptr<EnemyAI> m_flightAi;
    std::shared_ptr<EnemyAI> m_shootAi;

    float m_shootTimer = 0.0f;

    float m_health = 5.0f;

public:

    void setFlightAi(std::shared_ptr<EnemyAI> ai);
    void setShootAi(std::shared_ptr<EnemyAI> ai);

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

    bool canShoot();
    void shoot();

    std::shared_ptr<Transform> getTransform();
    void setTransform(std::shared_ptr<Transform> t);

    void takeDamage();

    std::shared_ptr<jt::Animation> getSprite() const;
};

#endif // QUASARRUSH_ENEMY_HPP
