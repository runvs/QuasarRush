#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"
#include <utility>

void Player::doCreate()
{
    m_shipSprite = std::make_shared<jt::Animation>();
    m_shipSprite->add("assets/ship.png", "idle", jt::Vector2u { 19, 19 }, { 0 }, 0.15f);
    m_shipSprite->add("assets/ship.png", "fly", jt::Vector2u { 19, 19 }, { 1, 2, 3, 4 }, 0.15f);
    m_shipSprite->play("idle");
    m_shipSprite->setOrigin(jt::Vector2 { 9.5f, 9.5f });

    m_flameSprite = std::make_shared<jt::Animation>();
    m_flameSprite->add("assets/shipFlame.png", "idle", jt::Vector2u { 16, 16 }, { 0 }, 0.15f);
    m_flameSprite->add(
        "assets/shipFlame.png", "fly", jt::Vector2u { 16, 16 }, { 1, 2, 3, 4 }, 0.15f);
    m_flameSprite->add(
        "assets/shipFlame.png", "flyBoost", jt::Vector2u { 16, 16 }, { 5, 6, 7, 8 }, 0.15f);
    m_flameSprite->play("idle");
    m_flameSprite->setOrigin(jt::Vector2 { 16.0f, 8.0f });

    m_transform = std::make_shared<Transform>();

    m_projectionShape = jt::dh::createRectShape(jt::Vector2 { 2.0f, 2.0f });
}

void Player::doUpdate(float const elapsed)
{
    updateMovement(elapsed);

    m_shootTimer -= elapsed;

    auto const px = jt::MathHelper::clamp(
        m_transform->position.x(), 0.0f, GP::GetScreenSize().x() - GP::PlayerHalfSize() * 2.0f);
    auto const py = jt::MathHelper::clamp(
        m_transform->position.y(), 0.0f, GP::GetScreenSize().y() - GP::PlayerHalfSize() * 2.0f);
    m_shipSprite->setRotation(m_transform->angle);
    m_shipSprite->setPosition(jt::Vector2 { px, py });
    m_shipSprite->update(elapsed);

    jt::Vector2 flameOffset { -9.5f, 0.0f };
    flameOffset = jt::MathHelper::rotateBy(flameOffset, -m_transform->angle);
    m_flameSprite->setPosition(m_shipSprite->getPosition() + flameOffset);
    m_flameSprite->setRotation(m_transform->angle);
    m_flameSprite->update(elapsed);
}

void Player::updateMovement(const float elapsed)
{
    auto const a = jt::MathHelper::deg2rad(m_transform->angle);
    jt::Vector2 const direction { cos(a), -sin(a) };

    auto const& keyboard = getGame()->input()->keyboard();
    if (keyboard->pressed(jt::KeyCode::W)) {
        bool flyBoost = keyboard->pressed(jt::KeyCode::LShift);

        m_shipSprite->play("fly");
        if (flyBoost) {
            m_flameSprite->play("flyBoost");
        } else {
            m_flameSprite->play("fly");
        }

        float const acceleration_factor = flyBoost ? GP::PlayerAccelerationBoostFactor() : 1.0f;
        m_transform->player_acceleration
            = direction * GP::PlayerAcceleration() * acceleration_factor;
    } else if (keyboard->justReleased(jt::KeyCode::W)) {
        m_shipSprite->play("idle");
        m_flameSprite->play("idle");
    } else {
        m_transform->player_acceleration = jt::Vector2 { 0.0f, 0.0f };
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

    m_shipSprite->draw(getGame()->getRenderTarget());
    m_flameSprite->draw(getGame()->getRenderTarget());
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
