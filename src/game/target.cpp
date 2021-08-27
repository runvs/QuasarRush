#include "target.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"

Target::Target(jt::Vector2 const& pos) { m_position = pos; }

jt::Vector2 Target::getPosition() const { return m_position; }

void Target::doCreate()
{
    m_shape = jt::dh::createRectShape(jt::Vector2 { 8.0, 8.0 }, jt::colors::Yellow);
    m_shape->setPosition(m_position);
}
void Target::doUpdate(float const elapsed) { m_shape->update(elapsed); }
void Target::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }
void Target::doKill() { }

