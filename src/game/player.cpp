#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include <utility>

void Player::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>();
    m_sprite->loadSprite("assets/ship.png");
    m_transform = std::make_shared<Transform>();

    m_projectionShape = jt::dh::createRectShape(jt::Vector2 { 2.0f, 2.0f });
}

void Player::doUpdate(float const elapsed)
{
    updateMovement(elapsed);

    m_sprite->setOrigin(jt::Vector2 { 5.0, 5.0 });
    m_sprite->setRotation(m_transform->angle);

    m_sprite->setPosition(m_transform->position);
    m_sprite->update(0.0f);
}

void Player::updateMovement(const float elapsed)
{
    auto const a = jt::MathHelper::deg2rad(m_transform->angle);
    jt::Vector2 const direction { cos(a), -sin(a) };

    auto const& keyboard = getGame()->input()->keyboard();
    if (keyboard->pressed(jt::KeyCode::W)) {
        float const acceleration_factor = keyboard->pressed(jt::KeyCode::LShift) ? GP::PlayerAccelerationBoostFactor() : 1.0f ;
        m_transform->acceleration += direction * GP::PlayerAcceleration() * acceleration_factor;
    }

    float const rotationSpeed = GP::PlayerRotationSpeed();
    if (keyboard->pressed(jt::KeyCode::A)) {
        m_transform->angle += rotationSpeed * elapsed;
    } else if (keyboard->pressed(jt::KeyCode::D)) {
        m_transform->angle -= rotationSpeed * elapsed;
    }
}

void Player::doDraw() const
{

    for (size_t i = 0U; i != m_projectionPoints.size(); ++i) {
        float const v =  255.0f - static_cast<float>(i) / m_projectionPoints.size() * 255.0f;
        m_projectionShape->setPosition(m_projectionPoints.at(i));
        m_projectionShape->setColor(jt::Color { 255, 255, 255, static_cast<std::uint8_t>(v) });
        m_projectionShape->update(0.1f);
        m_projectionShape->draw(getGame()->getRenderTarget());
    }

    m_sprite->draw(getGame()->getRenderTarget());
}
void Player::doKill() { }

std::shared_ptr<Transform> Player::getTransform() { return m_transform; }
void Player::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }

void Player::setProjectionPoints(std::vector<jt::Vector2>&& points)
{
    m_projectionPoints = std::move(points);
}
