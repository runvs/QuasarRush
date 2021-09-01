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
    m_transform->angle = jt::MathHelper::angleOf(m_transform->velocity);

    float missileTargetAcquisitionTime = 0.5f;
    float velocityDrag = 0.995f;

    auto v = m_transform->velocity;
    v =v * velocityDrag;
    m_transform->velocity = v;
    
    if (getAge() > missileTargetAcquisitionTime)
    {
        if (m_target.expired())
        {
            kill();
            return;
        }

        auto t = m_target.lock();

        auto const mp = m_transform->position;
        auto const tt = t->position + m_targetOffset;

        auto dif = tt-mp;

        jt::MathHelper::normalizeMe(dif);

        m_transform->player_acceleration = dif * 100.0f;
    }

    if (m_transform->position.x() < 0 || m_transform->position.x() > GP::GetScreenSize().x()) {
        kill();
    }
    if (m_transform->position.y() < 0 || m_transform->position.y() > GP::GetScreenSize().y()) {
        kill();
    }
    if (getAge() > 20.0f) {
        kill();
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
