#include "player.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"
#include "sprite.hpp"
#include "weapon_mg.hpp"
#include "weapon_rockets.hpp"
#include <memory>
#include <utility>

std::unique_ptr<WeaponInterface> createWeaponFromConfig(PlayerConfig config)
{
    if (config.weapon == WeaponTypeMg) {
        return std::make_unique<WeaponMg>();
    } else if (config.weapon == WeaponTypeRockets) {
        return std::make_unique<WeaponRockets>();
    } else {
        throw std::invalid_argument { "invalid weapon type in createWeaponFromConfig" };
    }
}

Player::Player(ShotSpawnInterface& shotSpawnInterface, SpawnTrailInterface& spawnTrailInterface,
    PlayerConfig& pc,
    std::shared_ptr<ObserverInterface<std::tuple<float, jt::Vector2>>> healthObserver,
    std::shared_ptr<ObserverInterface<float>> reloadObserver)
    : m_shotSpawnInterface { shotSpawnInterface }
    , m_spawnTrailInterface { spawnTrailInterface }
    , m_playerConfig { pc }
    , m_healthObserver { healthObserver }
    , m_reloadObserver { reloadObserver }
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
    m_glowOverlayFlame->setOrigin(jt::Vector2 { 16.0f, 16.0f });
    m_glowOverlayFlame->setOffset(jt::Vector2 { 16.0f, 16.0f });

    m_glowOverlayShip = std::make_shared<jt::Sprite>();
    m_glowOverlayShip->loadSprite("#g#128#120");
    m_glowOverlayShip->setColor(jt::Color { 36, 8, 119 });
    m_glowOverlayShip->setOrigin(jt::Vector2 { 64.0f, 64.0f });

    m_weapon = createWeaponFromConfig(m_playerConfig);

    m_health = 3.0f + m_playerConfig.hullLevel * 2.0f;
}

void Player::doUpdate(float const elapsed)
{
    updateMovement(elapsed);
    updateShooting(elapsed);

    updateSprite(elapsed);

    updateFlame(elapsed);

    m_healthObserver->notify(std::make_tuple(m_health, m_transform->position));

    bool playerOutOfBounds
        = m_transform->position.x() < -m_shipSprite->getLocalBounds().width() * 1.5f
        || m_transform->position.y() < -m_shipSprite->getLocalBounds().height() * 1.5f
        || m_transform->position.x() >= GP::GetScreenSize().x()
        || m_transform->position.y() >= GP::GetScreenSize().y();
    if (playerOutOfBounds) {
        takeDamage(elapsed*2.0f);
    }
}
void Player::updateFlame(float const elapsed)
{
    jt::Vector2 flameOffset { -9.5f, 0.0f };
    flameOffset = jt::MathHelper::rotateBy(flameOffset, -m_transform->angle);
    m_flameSprite->setPosition(m_shipSprite->getPosition() + flameOffset);
    m_flameSprite->setRotation(m_transform->angle);
    m_flameSprite->update(elapsed);

    m_glowOverlayFlame->setPosition(m_flameSprite->getPosition()
        - jt::Vector2 {
            m_flameSprite->getLocalBounds().width(), m_flameSprite->getLocalBounds().height() });
    m_glowOverlayFlame->update(elapsed);

    float const t = getAge();
    float a = (abs(cos(sin(t * 8.0f) + t * 9.0f)) * 0.75f + 0.25f)
        * ((getGame()->input()->keyboard()->pressed(jt::KeyCode::W)) ? 1.0f : 0.0f);
    jt::Color col { 255, 255, 255, static_cast<std::uint8_t>(a * 255) };
    m_glowOverlayFlame->setColor(col);
}

void Player::updateSprite(float const elapsed)
{

    auto const px = m_transform->position.x();
    auto const py = m_transform->position.y();

    m_shipSprite->setRotation(m_transform->angle);
    m_shipSprite->setPosition(jt::Vector2 { px, py });
    m_shipSprite->update(elapsed);

    m_glowOverlayShip->setPosition(m_transform->position);
    m_glowOverlayShip->update(elapsed);
}

void Player::updateShooting(float const elapsed)
{
    m_weapon->update(elapsed);
    m_reloadObserver->notify(m_weapon->getReloadCompletion());

    if (getGame()->input()->mouse()->pressed(jt::MouseButtonCode::MBLeft)) {
        m_weapon->shoot(m_transform->position, getGame()->input()->mouse()->getMousePositionWorld(),
            m_playerConfig, m_shotSpawnInterface);
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
        acceleration_factor += m_playerConfig.engineLevel * 0.25f;
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

std::shared_ptr<jt::Animation> Player::getSprite() const { return m_shipSprite; }

bool Player::isDead() const { return m_health <= 0; }
void Player::takeDamage(float damageValue) { m_health -= damageValue; }
