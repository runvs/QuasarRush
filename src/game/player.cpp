#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"
#include "game_properties.hpp"

void Player::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>();
    m_sprite->loadSprite("assets/ship.png");
    m_transform = std::make_shared<Transform>();
    m_transform->position = jt::Vector2 { 100, 100 };
    m_transform->velocity = jt::Vector2 { 20, 20 };
}

void Player::doUpdate(float const elapsed)
{
    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::W)) {
        auto const a =jt::MathHelper::deg2rad( m_transform->angle);
        jt::Vector2 const acc{cos(a), sin(a)};
        m_transform->acceleration += acc * GP::PlayerAcceleration();
    }

    float const rotationSpeed = GP::PlayerRotationSpeed();
    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::A)) {
        m_transform->angle += rotationSpeed * elapsed;
    } else if (getGame()->input()->keyboard()->pressed(jt::KeyCode::D)) {
        m_transform->angle -= rotationSpeed * elapsed;
    }

    m_sprite->setOrigin(jt::Vector2{5.0,5.0});
    m_sprite->setRotation(jt::MathHelper::deg2rad(m_transform->angle));

    m_sprite->setPosition(m_transform->position);
    m_sprite->update(0.0f);

}

void Player::doDraw() const {    m_sprite->draw(getGame()->getRenderTarget()); }
void Player::doKill() { }

std::shared_ptr<Transform> Player::getTransform() { return m_transform; }
void Player::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
