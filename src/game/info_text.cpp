#include "info_text.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "text.hpp"
#include <line.hpp>

InfoText::InfoText(
    std::shared_ptr<jt::DrawableInterface> drawable, std::string text, eInfoTextAlign align)
    : m_objectToTrack { drawable }
    , m_textstring { text }
    , m_diagonalVector { 60.0f, 16.0f }
    , m_align{align}
    , m_color { jt::colors::White }

{
    if (m_align == eInfoTextAlign::RightUp)
    {
        m_diagonalVector= jt::Vector2{60.0f, -16.0f};
    }
    else if (m_align == eInfoTextAlign::LeftDown)
    {
        m_diagonalVector= jt::Vector2{-60.0f, -16.0f};
    }
    else if (m_align == eInfoTextAlign::LeftUp)
    {
        m_diagonalVector= jt::Vector2{-60.0f, 16.0f};
    }
}

void InfoText::setColor(jt::Color const& col) { m_color = col; }
jt::Color InfoText::getColor() const { return m_color; }

void InfoText::doCreate()
{
    m_text = jt::dh::createText(getGame()->getRenderTarget(), m_textstring, 10);
    m_text->SetTextAlign(jt::Text::TextAlign::LEFT);
}
void InfoText::doUpdate(float const elapsed)
{
    if (m_objectToTrack.expired()) {
        kill();
        return;
    }

    auto target = m_objectToTrack.lock();

    auto const targetPosition = target->getPosition();

    jt::Vector2 offset ;
    if(m_align == eInfoTextAlign::RightDown)
    {
        offset = jt::Vector2 { 10.0f, 0.0f };
    }
    else if (m_align == eInfoTextAlign::RightUp)
    {
        offset = jt::Vector2 { 10.0f, -24.0f };
    }
    else if (m_align == eInfoTextAlign::LeftDown)
    {
        offset = jt::Vector2 { -10.0f, 0.0f };
    }
    else if (m_align == eInfoTextAlign::LeftUp)
    {
        offset = jt::Vector2 { -10.0f, -24.0f };
    }
    auto textPosition = targetPosition + m_diagonalVector + offset;
    if (m_align == eInfoTextAlign::LeftDown || m_align == eInfoTextAlign::LeftUp)
    {
        textPosition.x() -= m_text->getLocalBounds().width();
    }

    m_text->setPosition(textPosition);
    m_text->setColor(m_color);
    m_text->update(elapsed);
}
void InfoText::doDraw() const
{
    if (!isAlive()) {
        return;
    }

    auto target = m_objectToTrack.lock();

    jt::Vector2 offset ;
    if(m_align == eInfoTextAlign::RightDown)
    {
        offset = jt::Vector2 { 10.0f, 12.0f };
    }
    else if (m_align == eInfoTextAlign::RightUp)
    {
        offset = jt::Vector2 { 10.0f, -12.0f };
    }
    else if (m_align == eInfoTextAlign::LeftDown)
    {
        offset = jt::Vector2 { -10.0f, 12.0f };
    }
    else if (m_align == eInfoTextAlign::LeftUp)
    {
        offset = jt::Vector2 { -10.0f, -12.0f };
    }
    auto const targetPosition = target->getPosition() + offset;

    jt::Line lineDiag(m_diagonalVector);
    lineDiag.setPosition(targetPosition);
    lineDiag.update(0.1f);
    lineDiag.setColor(m_color);
    lineDiag.draw(getGame()->getRenderTarget());

    jt::Line lineUnder(jt::Vector2 { m_text->getGlobalBounds().width(), 0 });
    lineUnder.setPosition(m_text->getPosition() + jt::Vector2 { 0.0f, 12.0f });
    lineUnder.update(0.1f);
    lineUnder.setColor(m_color);
    lineUnder.draw(getGame()->getRenderTarget());

    m_text->draw(getGame()->getRenderTarget());
}
void InfoText::doKill() { }
