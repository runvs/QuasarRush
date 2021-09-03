#include "impact_explosion.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"

void ImpactExplosion::doCreate()
{
    m_sprite = std::make_shared<jt::Animation>();
    m_sprite->add("assets/impact.png", "explode", jt::Vector2u { 8, 8 },
        jt::MathHelper::vectorBetween<unsigned int>(0, m_numberOfFrames - 1), m_frameTime);
    m_sprite->play("explode");
    m_sprite->setOrigin(jt::Vector2 { 4.0, 4.0 });
}

void ImpactExplosion::doUpdate(float const elapsed)
{
    m_sprite->update(elapsed);

    if (getAge() > m_frameTime * m_numberOfFrames) {
        kill();
    }
}

void ImpactExplosion::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }

std::shared_ptr<jt::Animation> ImpactExplosion::getAnimation() const { return m_sprite; }