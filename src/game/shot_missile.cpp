#include "shot_missile.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"
#include "game_properties.hpp"

void ShotMissile::doCreate() {
    m_transform = std::make_shared<Transform>();

    m_sprite = std::make_shared<jt::Sprite>();
    m_sprite->loadSprite("assets/shot.png");

    m_targetOffset = jt::Random::getRandomPointin(jt::Rect{-15, -15, 30, 30});
}

void ShotMissile::doUpdate(float const elapsed) {

    m_sprite->setPosition(m_transform->position);
    m_sprite->setRotation(m_transform->angle);
    m_sprite->update(elapsed);


    if (getAge() < 0.5f)
    {
        m_transform->angle = jt::MathHelper::angleOf(m_transform->velocity);
        auto v = m_transform->velocity;
        auto vN = v;
        jt::MathHelper::normalizeMe(vN);
        v -= elapsed * vN * (GP::ShotSpeed()* 0.99f)/1.0f;
        m_transform->velocity = v;
    }
    else
    {
        if (m_target.expired())
        {
            kill();
            return;
        }
        auto t = m_target.lock();

        m_transform->angle = jt::MathHelper::angleOf(m_transform->velocity);

        auto const mp = m_transform->position;
        auto const tt = t->position + m_targetOffset;

        auto dif = tt-mp;

        jt::MathHelper::normalizeMe(dif);
        auto v = m_transform->velocity;
        auto vN = v;
        v = v * 0.995f;
        m_transform->velocity = v;

        m_transform->player_acceleration = dif * 100.0f;

    }






}

void ShotMissile::doDraw() const {
    m_sprite->draw(getGame()->getRenderTarget());
}

std::shared_ptr<Transform> ShotMissile::getTransform() { return m_transform; }
void ShotMissile::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }

bool ShotMissile::getFiredByPlayer() const { return m_firedByPlayer; }
void ShotMissile::setFiredByPlayer(bool value) { m_firedByPlayer = value; }
void ShotMissile::hit() { kill(); }

void ShotMissile::setTarget(std::weak_ptr<Transform> target) { m_target = target; }
