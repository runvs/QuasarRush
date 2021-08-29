#include "target.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "tween_alpha.hpp"
#include "tween_scale.hpp"

Target::Target(jt::Vector2 const& pos) { m_position = pos; }

jt::Vector2 Target::getPosition() const { return m_position; }

void Target::doCreate()
{
    m_shape = jt::dh::createRectShape(jt::Vector2 { 8.0, 8.0 }, jt::colors::Yellow);
    m_shape->setPosition(m_position);
    m_shape->setOrigin(jt::Vector2 { 4.0f, 4.0f });

    m_glow = std::make_shared<jt::Sprite>();
    m_glow->loadSprite("#g#64#64");
    m_glow->setOrigin(jt::Vector2 { 32, 32 });

    auto color = jt::colors::Yellow;
    color.a() = 120;
    m_glow->setColor(color);
    m_glow->setPosition(m_position);
}

void Target::doUpdate(float const elapsed)
{
    m_shape->update(elapsed);
    m_glow->update(elapsed);
}
void Target::doDraw() const
{
    m_shape->draw(getGame()->getRenderTarget());
    m_glow->draw(getGame()->getRenderTarget());
}
void Target::doKill() { }

std::vector<std::shared_ptr<jt::TweenBase>> Target::hit()
{
    m_hasBeenHit = true;
    std::vector<std::shared_ptr<jt::TweenBase>> tweens;
    auto ts = jt::TweenScale<jt::Shape>::create(
        m_shape, 0.65f, jt::Vector2 { 1.0f, 1.0f }, jt::Vector2 { 2.5f, 2.5f });
    ts->addCompleteCallback([this]() { kill(); });
    tweens.push_back(ts);

    auto ta = jt::TweenAlpha<jt::Shape>::create(m_shape, 0.65f, 255U, 0U);
    tweens.push_back(ta);

    auto ta2 = jt::TweenAlpha<jt::Sprite>::create(m_glow, 0.65f, m_glow->getColor().a(), 0U);
    tweens.push_back(ta2);

    return tweens;
}
bool Target::hasBeenHit() const { return m_hasBeenHit; }
std::shared_ptr<jt::Shape> Target::getShape() const { return m_shape; }
