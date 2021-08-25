#include "shot.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"

void Shot::doCreate()
{
    m_transform = std::make_shared<Transform>();
    m_shape = jt::dh::createRectShape(jt::Vector2 { 2.0f, 2.0f }, jt::colors::Cyan);
}

void Shot::doUpdate(float const elapsed)
{
    m_shape->setPosition(m_transform->position);
    m_shape->update(elapsed);

    if (m_transform->position.x() < 0 || m_transform->position.x() > GP::GetScreenSize().x())
    {
        kill();
    }
    if (m_transform->position.y() < 0 || m_transform->position.y() > GP::GetScreenSize().y())
    {
        kill();
    }
    if (getAge() > 60.0f)
    {
        kill();
    }
}
void Shot::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }
void Shot::doKill() { }

std::shared_ptr<Transform> Shot::getTransform() { return m_transform; }

void Shot::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
