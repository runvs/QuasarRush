#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game.hpp"


void Player::doCreate()
{
    m_shape = jt::dh::createRectShape({ 10.0f, 10.0f }, jt::colors::White);
    m_transform = std::make_shared<Transform>();
    m_transform->position = jt::Vector2 { 100, 100 };
    m_transform->velocity= jt::Vector2 {20 , 20 };
}

void Player::doUpdate(float const elapsed)
{
    float const a = getAge();
    //m_transform->position = jt::Vector2(200 + sin(a / 2) * 50, 200 + cos(a / 2) * 50);

    m_shape->setPosition(m_transform->position);
    m_shape->update(elapsed);
}

void Player::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }
void Player::doKill() {}

std::shared_ptr<Transform> Player::getTransform() { return m_transform; }
