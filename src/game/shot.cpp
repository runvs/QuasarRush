#include "shot.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"

void Shot::doCreate()
{
    m_transform = std::make_shared<Transform>();
    m_shape = jt::dh::createRectShape(jt::Vector2 { 2.0f, 2.0f }, jt::colors::Cyan);
}

void Shot::doUpdate(float const elapsed)
{
    m_shape->setPosition(m_transform->position);
    m_shape->update(elapsed);
}
void Shot::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }
void Shot::doKill() { }

std::shared_ptr<Transform> Shot::getTransform() { return m_transform; }

void Shot::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
