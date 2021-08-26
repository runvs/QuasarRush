#include "planet.hpp"
#include "game_interface.hpp"

void Planet::doCreate()
{
    m_sprite = std::make_shared<jt::Animation>();
    m_sprite->add("assets/planet.png", "idle", jt::Vector2u { 32, 32 },
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, 0.15f);
    m_sprite->play("idle");
    m_transform = std::make_shared<Transform>();
}

void Planet::doUpdate(float const elapsed)
{
    m_sprite->setOrigin(jt::Vector2 { 16.0, 16.0 });
    m_sprite->setRotation(m_transform->angle);

    m_sprite->setPosition(m_transform->position);
    m_sprite->update(elapsed);
}

void Planet::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }
void Planet::doKill() { }

std::shared_ptr<Transform> Planet::getTransform() { return m_transform; }
void Planet::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
