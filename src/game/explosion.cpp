#include "explosion.hpp"
#include "game_interface.hpp"
#include "math_helper.hpp"

void Explosion::doCreate()
{
    m_sprite = std::make_shared<jt::Animation>();
    m_sprite->add("assets/explosion.png", "explode", jt::Vector2u { 20, 20 },
        jt::MathHelper::vectorBetween<unsigned int>(0, m_numberOfFrames - 1), m_frameTime);
    m_sprite->play("explode");
    m_sprite->setOrigin(jt::Vector2 { 10.0, 10.0 });
}

void Explosion::doUpdate(float const elapsed)
{
    m_sprite->update(elapsed);

    if (getAge() > m_frameTime * m_numberOfFrames) {
        kill();
    }
}

void Explosion::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }

std::shared_ptr<jt::Animation> Explosion::getAnimation() const { return m_sprite; }