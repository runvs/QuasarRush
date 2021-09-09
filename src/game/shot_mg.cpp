#include "shot_mg.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

ShotMg::ShotMg(ExplosionSpawnInterface& explosionSpawnInterface)
    : m_explosionSpawnInterface { explosionSpawnInterface }
{
}

void ShotMg::doCreate()
{
    m_transform = std::make_shared<Transform>();
    m_sprite = std::make_shared<jt::Sprite>();
    m_sprite->loadSprite("assets/shot.png");
}

void ShotMg::doUpdate(float const elapsed)
{
    m_sprite->setPosition(m_transform->position);
    m_transform->angle = jt::MathHelper::angleOf(m_transform->velocity);
    m_sprite->setRotation(-m_transform->angle);
    m_sprite->update(elapsed);

    float mgShotDecayStart = 3.0f;
    float mgShotDecayTime = 2.5f;
    if (getAge() > mgShotDecayStart)
    {
        auto alphaValue = 255.0f * (1.0f - (getAge() - mgShotDecayStart)/ mgShotDecayTime);
        alphaValue = jt::MathHelper::clamp(alphaValue,0.0f, 255.0f);
        auto col = m_sprite->getColor();

        col.a() = static_cast<uint8_t>(alphaValue);
        m_sprite->setColor(col);
    }

    if (getAge() > mgShotDecayStart + mgShotDecayStart) {
        kill();
    }
}
void ShotMg::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }

std::shared_ptr<Transform> ShotMg::getTransform() { return m_transform; }
void ShotMg::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
bool ShotMg::getFiredByPlayer() const { return m_fired_by_player; }
void ShotMg::setFiredByPlayer(bool value) { m_fired_by_player = value; }
void ShotMg::hit()
{
    m_explosionSpawnInterface.spawnImpactExplosion(m_transform->position);
    kill();
}
float ShotMg::getDamageValue() { return 0.25f; }
std::shared_ptr<jt::DrawableInterface> ShotMg::getDrawable()
{
    return m_sprite;
}
