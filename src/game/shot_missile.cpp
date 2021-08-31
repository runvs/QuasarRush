#include "shot_missile.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"
#include "game_properties.hpp"

void ShotMissile::doCreate() {
    m_transform = std::make_shared<Transform>();

    m_sprite = std::make_shared<jt::Sprite>();
    m_sprite->loadSprite("assets/shot.png");
}

void ShotMissile::doUpdate(float const elapsed) {

    m_sprite->setPosition(m_transform->position);
    m_sprite->setRotation(m_transform->angle);
    m_sprite->update(elapsed);


    if (getAge() < 2.0f)
    {
        m_transform->angle = jt::MathHelper::angleOf(m_transform->velocity);
        auto v = m_transform->velocity;
        auto vN = v;
        jt::MathHelper::normalizeMe(vN);
        v -= elapsed * vN * (GP::ShotSpeed()* 0.99f)/2.0f;
        auto const l = jt::MathHelper::length(v);
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
        auto const tt = t->position;

        auto dif = tt-mp;

        jt::MathHelper::normalizeMe(dif);

        m_transform->player_acceleration = dif * 10.0f;
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
