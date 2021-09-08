#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"
#include "sprite.hpp"
#include <utility>

Player::Player(ShotSpawnInterface& shotSpawnInterface, SpawnTrailInterface& spawnTrailInterface,
    PlayerConfig& pc)
    : m_shotSpawnInterface { shotSpawnInterface }, m_spawnTrailInterface{spawnTrailInterface}, m_playerConfig{pc}
{
}

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

    m_glowOverlayFlame = std::make_shared<jt::Sprite>();
    m_glowOverlayFlame->loadSprite("#g#32#100");
    m_glowOverlayFlame->setOrigin(jt::Vector2{16.0f,16.0f});
    m_glowOverlayFlame->setOffset(jt::Vector2{16.0f, 16.0f});

    m_glowOverlayShip = std::make_shared<jt::Sprite>();
    m_glowOverlayShip->loadSprite("#g#128#120");
    m_glowOverlayShip->setColor(jt::Color{36,8,119});
    m_glowOverlayShip->setOrigin(jt::Vector2{64.0f,64.0f});
}

void Player::doUpdate(float const elapsed)
{
    updateMovement(elapsed);
    updateShooting(elapsed);

    updateSprite(elapsed);

    updateFlame(elapsed);
}
void Player::updateFlame(float const elapsed)
{
    jt::Vector2 flameOffset { -9.5f, 0.0f };
    flameOffset = jt::MathHelper::rotateBy(flameOffset, -m_transform->angle);
    m_flameSprite->setPosition(m_shipSprite->getPosition() + flameOffset);
    m_flameSprite->setRotation(m_transform->angle);
    m_flameSprite->update(elapsed);

    m_glowOverlayFlame->setPosition(m_flameSprite->getPosition() - jt::Vector2{ m_flameSprite->getLocalBounds().width(), m_flameSprite->getLocalBounds().height()});
    m_glowOverlayFlame->update(elapsed);

    float const t = getAge();
    float a = ( abs(cos(sin(t * 8.0f) + t * 9.0f)) * 0.75f + 0.25f) * ((getGame()->input()->keyboard()->pressed(jt::KeyCode::W))?1.0f : 0.0f);
    jt::Color col{255,255,255, static_cast<std::uint8_t>(a * 255)};
    m_glowOverlayFlame->setColor(col);
}

void Player::updateSprite(float const elapsed)
{
    auto const px = jt::MathHelper::clamp(m_transform->position.x(),
        static_cast<float>(GP::PlayerHalfSize()), GP::GetScreenSize().x() - GP::PlayerHalfSize());
    auto const py = jt::MathHelper::clamp(m_transform->position.y(),
        static_cast<float>(GP::PlayerHalfSize()), GP::GetScreenSize().y() - GP::PlayerHalfSize());

    m_shipSprite->setRotation(m_transform->angle);
    m_shipSprite->setPosition(jt::Vector2 { px, py });
    m_shipSprite->update(elapsed);

    m_glowOverlayShip->setPosition(m_transform->position);
    m_glowOverlayShip->update(elapsed);
}

void Player::updateShooting(float const elapsed)
{
    m_shootTimer -= elapsed;

    if (getGame()->input()->mouse()->pressed(jt::MouseButtonCode::MBLeft)) {
        if (canShoot()) {
            shoot();
        }
    }
}

void Player::updateMovement(const float elapsed)
{
    auto const a = jt::MathHelper::deg2rad(m_transform->angle);
    jt::Vector2 const direction { cos(a), -sin(a) };

    auto const& keyboard = getGame()->input()->keyboard();
    if (keyboard->pressed(jt::KeyCode::W)) {
        bool const flyBoost = keyboard->pressed(jt::KeyCode::LShift);

        m_shipSprite->play("fly");
        jt::Vector2 flameOffset { -9.5f, 0.0f };
        flameOffset = jt::MathHelper::rotateBy(flameOffset, -m_transform->angle);
        m_spawnTrailInterface.spawnTrail(m_transform->position + flameOffset);
        if (flyBoost) {
            m_flameSprite->play("flyBoost");
        } else {
            m_flameSprite->play("fly");
        }

        float acceleration_factor = flyBoost ? GP::PlayerAccelerationBoostFactor() : 1.0f;
        acceleration_factor +=  m_playerConfig.engineLevel * 0.25f;
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


    m_glowOverlayShip->draw(getGame()->getRenderTarget());
    m_shipSprite->draw(getGame()->getRenderTarget());

    m_flameSprite->draw(getGame()->getRenderTarget());
    m_glowOverlayFlame->draw(getGame()->getRenderTarget());
}
void Player::doKill() { }

std::shared_ptr<Transform> Player::getTransform() { return m_transform; }
void Player::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }

void Player::setProjectionPoints(std::vector<jt::Vector2>&& points)
{
    m_projectionPoints = std::move(points);
}
bool Player::canShoot() { return m_shootTimer <= 0; }

void Player::shoot()
{
    auto const mouse_position = getGame()->input()->mouse()->getMousePositionScreen();
    auto aim_direction = mouse_position - m_transform->position;
    jt::MathHelper::normalizeMe(aim_direction);
    m_shotCounter++;

    float shotTimeFactor = 1.0f - (static_cast<float>(m_playerConfig.weaponLevel)-1.0f) * 0.1f;
    shotTimeFactor = jt::MathHelper::clamp(shotTimeFactor, 0.2f, 1.0f);

    // TODO Refactor to avoid ugly if statement. Use proper OOP
    if (m_playerConfig.weapon == WeaponMg) {
        m_shootTimer = GP::PlayerShootTimerMg() * shotTimeFactor;
        m_shootTimerMax = m_shootTimer;
        jt::Vector2 const orthogonal_aim_direction{aim_direction.y(), -aim_direction.x()};
        auto startPos = m_transform->position + 6.0f * orthogonal_aim_direction * ((m_shotCounter%2 == 0) ? -1.0f : 1.0f);
        m_shotSpawnInterface.spawnShotMg(startPos, aim_direction, true);
    }
    else if (m_playerConfig.weapon == WeaponRockets) {
        m_shootTimer = GP::PlayerShootTimerMissile() * shotTimeFactor;
        m_shootTimerMax = m_shootTimer;
        m_shotSpawnInterface.spawnShotMissile(m_transform->position, aim_direction, true);
    }
    else
    {
        std::cerr << "unsupported weapon" << m_playerConfig.weapon << std::endl;
    }
}
std::shared_ptr<jt::Animation> Player::getSprite() const { return m_shipSprite; }
