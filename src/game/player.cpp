#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include <utility>

void Player::doCreate()
{
    m_sprite = std::make_shared<jt::Animation>();
    m_sprite->add("assets/ship.png", "idle", jt::Vector2u { 30, 16 }, { 0 }, 0.15f);
    m_sprite->add("assets/ship.png", "beginFly", jt::Vector2u { 30, 16 }, { 1, 2, 3, 4 }, 0.05f);
    m_sprite->add("assets/ship.png", "fly", jt::Vector2u { 30, 16 }, { 5, 6, 7, 8 }, 0.15f);
    m_sprite->play("idle");

    m_transform = std::make_shared<Transform>();

    m_projectionShape = jt::dh::createRectShape(jt::Vector2 { 2.0f, 2.0f });
}

void Player::doUpdate(float const elapsed)
{
    updateMovement(elapsed);

    m_shootTimer -= elapsed;

    m_sprite->setOrigin(jt::Vector2 { 15.0, 8.0 });
    m_sprite->setRotation(m_transform->angle);

    m_sprite->setPosition(m_transform->position);
    m_sprite->update(elapsed);
}

void Player::updateMovement(const float elapsed)
{
    auto const a = jt::MathHelper::deg2rad(m_transform->angle);
    jt::Vector2 const direction { cos(a), -sin(a) };

    auto const& keyboard = getGame()->input()->keyboard();
    if (keyboard->pressed(jt::KeyCode::W)) {
        m_sprite->play("fly");
        float const acceleration_factor
            = keyboard->pressed(jt::KeyCode::LShift) ? GP::PlayerAccelerationBoostFactor() : 1.0f;
        m_transform->acceleration += direction * GP::PlayerAcceleration() * acceleration_factor;
    } else if (keyboard->justReleased(jt::KeyCode::W)) {
        m_sprite->play("idle");
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
        float const v = 255.0f - static_cast<float>(i) / m_projectionPoints.size() * 255.0f;
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
bool Player::canShoot() { return m_shootTimer <= 0; }
void Player::shoot() { m_shootTimer = GP::PlayerShootTimer(); }
