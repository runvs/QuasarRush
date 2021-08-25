#include "enemy.hpp"
#include "game_interface.hpp"
#include "drawable_helpers.hpp"
#include "game_properties.hpp"

void Enemy::doCreate()
{
    m_shape = jt::dh::createRectShape(jt::Vector2{10.0f, 10.0f});
    m_transform = std::make_shared<Transform>();
}

void Enemy::doUpdate(float const elapsed)
{
    m_shootTimer -= elapsed;

    m_shape->setOrigin(jt::Vector2 { 5.0, 5.0 });
    m_shape->setRotation(m_transform->angle);

    m_shape->setPosition(m_transform->position);
    m_shape->update(0.0f);
}


void Enemy::doDraw() const
{
    m_shape->draw(getGame()->getRenderTarget());
}
void Enemy::doKill() { }

std::shared_ptr<Transform> Enemy::getTransform() { return m_transform; }
void Enemy::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }

bool Enemy::canShoot() { return m_shootTimer <= 0; }
void Enemy::shoot() { m_shootTimer = GP::EnemyShootTimer(); }
