#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

void Player::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>();
    m_sprite->loadSprite("assets/ship.png");
    m_transform = std::make_shared<Transform>();

    m_projectionShape = jt::dh::createRectShape(jt::Vector2 { 2.0f, 2.0f });
}

void Player::doUpdate(float const elapsed)
{
    auto const a = jt::MathHelper::deg2rad(m_transform->angle);
    jt::Vector2 const acc { cos(a), -sin(a) };

//    std::cout << m_transform->angle << " " << a << " " << acc.x() << " " << acc.y() << std::endl;

    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::W)) {

        m_transform->acceleration += acc * GP::PlayerAcceleration();
    }

    float const rotationSpeed = GP::PlayerRotationSpeed();
    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::A)) {
        m_transform->angle += rotationSpeed * elapsed;
    } else if (getGame()->input()->keyboard()->pressed(jt::KeyCode::D)) {
        m_transform->angle -= rotationSpeed * elapsed;
    }

    m_sprite->setOrigin(jt::Vector2 { 5.0, 5.0 });
    m_sprite->setRotation(m_transform->angle);

    m_sprite->setPosition(m_transform->position);
    m_sprite->update(0.0f);
}

void Player::doDraw() const
{

    for (auto& p : m_projectionPoints) {
//        std::cout << p.x() << " " << p.y() << std::endl;
        m_projectionShape->setPosition(p);
        m_projectionShape->update(0.1f);
        m_projectionShape->draw(getGame()->getRenderTarget());
    }
    std::cout << std::endl;

    m_sprite->draw(getGame()->getRenderTarget());
}
void Player::doKill() { }

std::shared_ptr<Transform> Player::getTransform() { return m_transform; }
void Player::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
