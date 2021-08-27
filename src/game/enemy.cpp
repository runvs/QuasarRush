#include "enemy.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

void Enemy::doCreate()
{
    m_shipSprite = std::make_shared<jt::Animation>();
    m_shipSprite->add("assets/enemy.png", "idle", jt::Vector2u { 16, 16 }, { 0 }, 0.15f);
    m_shipSprite->add("assets/enemy.png", "fly", jt::Vector2u { 16, 16 }, { 1 }, 0.15f);
    m_shipSprite->play("idle");
    m_shipSprite->setOrigin(jt::Vector2 { 8.0, 8.0 });

    m_flameSprite = std::make_shared<jt::Animation>();
    m_flameSprite->add("assets/enemyFlame.png", "idle", jt::Vector2u { 16, 16 }, { 0 }, 0.15f);
    m_flameSprite->add(
        "assets/enemyFlame.png", "fly", jt::Vector2u { 16, 16 }, { 1, 2, 3, 4, 5 }, 0.15f);
    m_flameSprite->play("idle");
    m_flameSprite->setOrigin(jt::Vector2 { 8.0, 8.0 });

    m_transform = std::make_shared<Transform>();
}

void Enemy::doUpdate(float const elapsed)
{
    m_shootTimer -= elapsed;

    m_shipSprite->setRotation(m_transform->angle);
    m_shipSprite->setPosition(m_transform->position);
    m_shipSprite->update(elapsed);

    jt::Vector2 flameOffset { -8.0f, 0.0f };
    flameOffset = jt::MathHelper::rotateBy(flameOffset, -m_transform->angle);
    m_flameSprite->setPosition(m_shipSprite->getPosition() + flameOffset);
    m_flameSprite->setRotation(m_transform->angle);
    m_flameSprite->update(elapsed);
}

void Enemy::doDraw() const { m_shipSprite->draw(getGame()->getRenderTarget()); }
void Enemy::doKill() { }

std::shared_ptr<Transform> Enemy::getTransform() { return m_transform; }
void Enemy::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }

bool Enemy::canShoot() { return m_shootTimer <= 0; }
void Enemy::shoot() { m_shootTimer = GP::EnemyShootTimer(); }

void Enemy::takeDamage()
{
    m_health -= 1;
    if (m_health <= 0) {
        kill();
    }
}
