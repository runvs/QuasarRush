#include "bar.hpp"
#include <cassert>
#include <iostream>

namespace jt {

Bar::Bar(float width, float height, bool horizontal)
    : m_valueMax { 1.0f }
    , m_valueCurrent { 0.0f }
    , m_width { width }
    , m_height { height }
    , m_horizontal { horizontal }
{
    m_shapeFull = std::make_shared<jt::Shape>();
    m_shapeFull->makeRect(jt::Vector2 { m_width, m_height });
    m_shapeFull->setColor(jt::Color { 150, 150, 150 });

    m_shapeProgress = std::make_shared<jt::Shape>();
    if (m_horizontal) {
        m_shapeProgress->makeRect(jt::Vector2 { m_width, m_height * 0.9f });
        m_shapeProgress->setPosition(jt::Vector2 { 0, m_height * 0.1f });
    } else {
        m_shapeProgress->makeRect(jt::Vector2 { m_width - 2, m_height });
        m_shapeProgress->setPosition(jt::Vector2 { 0 + 1, m_height });
    }
    m_shapeProgress->setColor(jt::Color { 255, 255, 255 });
}

void Bar::setFrontColor(jt::Color const& col) { m_shapeProgress->setColor(col); }
void Bar::setBackColor(jt::Color const& col) { m_shapeFull->setColor(col); }

void Bar::setCurrentValue(float value)
{
    m_valueCurrent = value;
    if (m_valueCurrent < 0) {
        m_valueCurrent = 0;
    } else if (m_valueCurrent > m_valueMax) {
        m_valueCurrent = m_valueMax;
    }
}

float Bar::getCurrentValue() const { return m_valueCurrent; }

void Bar::setMaxValue(float max)
{
    assert(max >= 0);
    m_valueMax = max;
}

float Bar::getMaxValue() const { return m_valueMax; }

float Bar::getValueFraction() const { return m_valueCurrent / m_valueMax; }

void Bar::doDraw(std::shared_ptr<jt::renderTarget> const sptr) const
{
    m_shapeFull->draw(sptr);
    m_shapeProgress->draw(sptr);
}

void Bar::doDrawFlash(std::shared_ptr<jt::renderTarget> const /*sptr*/) const { }
void Bar::doDrawShadow(std::shared_ptr<jt::renderTarget> const sptr) const
{
    jt::Vector2 const oldPos = m_shapeFull->getPosition();
    jt::Color const oldCol = m_shapeFull->getColor();

    m_shapeFull->setPosition(oldPos + getShadowOffset());
    m_shapeFull->setColor(getShadowColor());
    m_shapeFull->draw(sptr);

    m_shapeFull->setPosition(oldPos);
    m_shapeFull->setColor(oldCol);
}

void Bar::doUpdate(float elapsed)
{
    float const value = static_cast<float>(m_valueCurrent) / m_valueMax;
    auto const scaleFullShape = m_shapeFull->getScale();
    if (m_horizontal) {
        m_shapeProgress->setScale(jt::Vector2 { value * scaleFullShape.x(), scaleFullShape.y() });
    } else {
        m_shapeProgress->setScale(jt::Vector2 { scaleFullShape.x(), value * scaleFullShape.y() });
        m_shapeProgress->setPosition(jt::Vector2 { m_shapeFull->getPosition().x() + 1,
            m_shapeFull->getPosition().y() + (1 - value) * m_height });
    }
    m_shapeFull->update(elapsed);
    m_shapeProgress->update(elapsed);
}
void Bar::doFlash(float t, jt::Color col) { m_shapeFull->flash(t, col); }
void Bar::doRotate(float /*rot*/) { }

void Bar::setColor(jt::Color const& col) { setFrontColor(col); }
const jt::Color Bar::getColor() const { return m_shapeProgress->getColor(); }

void Bar::setPosition(jt::Vector2 const& pos)
{
    m_shapeFull->setPosition(pos);
    m_shapeProgress->setPosition(pos + jt::Vector2 { 0, m_height * 0.1f });
}

const jt::Vector2 Bar::getPosition() const { return m_shapeFull->getPosition(); }

// sf::Transform const Bar::getTransform() const { return m_shapeFull.getTransform(); }
jt::Rect const Bar::getGlobalBounds() const { return m_shapeFull->getGlobalBounds(); }
jt::Rect const Bar::getLocalBounds() const { return m_shapeFull->getLocalBounds(); }

void Bar::setFlashColor(jt::Color const& col) { m_shapeFull->setFlashColor(col); }
const jt::Color Bar::getFlashColor() const { return jt::colors::White; }

void Bar::setScale(jt::Vector2 const& scale)
{
    m_shapeFull->setScale(scale);
    m_shapeProgress->setScale(scale);
}
const jt::Vector2 Bar::getScale() const { return m_shapeFull->getScale(); }

void Bar::setOrigin(jt::Vector2 const& origin) { m_shapeFull->setOrigin(origin); }
const jt::Vector2 Bar::getOrigin() const { return m_shapeFull->getOrigin(); }

} // namespace jt
