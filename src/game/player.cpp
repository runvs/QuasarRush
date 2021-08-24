#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"

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
    float const a = getAge();
    // m_transform->position = jt::Vector2(200 + sin(a / 2) * 50, 200 + cos(a / 2) * 50);

    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::W)) { }

    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::A)) {
        m_transform->angle += 36.0f;
    } else if (getGame()->input()->keyboard()->pressed(jt::KeyCode::D)) {
        m_transform->angle -= 36.0f;
    }

    m_sprite->setRotation(jt::MathHelper::deg2rad(m_transform->angle));

    m_sprite->setPosition(m_transform->position);
    m_sprite->update(elapsed);
}

void Player::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }
void Player::doKill() { }

std::shared_ptr<Transform> Player::getTransform() { return m_transform; }
void Player::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
