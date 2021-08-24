#include "planet.hpp"
#include "game_interface.hpp"

void Planet::doCreate()
{
    m_sprite = std::make_shared<jt::Sprite>();
    m_sprite->loadSprite("assets/planet.png");
    m_transform = std::make_shared<Transform>();
}

void Planet::doUpdate(float const elapsed)
{
    m_sprite->setOrigin(jt::Vector2 { 5.0, 5.0 });
    m_sprite->setRotation(m_transform->angle);

    m_sprite->setPosition(m_transform->position);
    m_sprite->update(0.0f);
}

void Planet::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }
void Planet::doKill() { }

std::shared_ptr<Transform> Planet::getTransform() { return m_transform; }
void Planet::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
